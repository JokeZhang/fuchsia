// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fuchsia/cobalt/cpp/fidl.h>
#include <fuchsia/devicesettings/cpp/fidl.h>
#include <fuchsia/process/lifecycle/cpp/fidl.h>
#include <fuchsia/settings/cpp/fidl.h>
#include <fuchsia/sys/cpp/fidl.h>
#include <lib/async/default.h>
#include <lib/fdio/directory.h>
#include <lib/sys/cpp/file_descriptor.h>
#include <lib/vfs/cpp/pseudo_dir.h>
#include <lib/vfs/cpp/pseudo_file.h>

#include "lib/sys/cpp/testing/test_with_environment.h"
#include "src/lib/files/directory.h"
#include "src/lib/files/file.h"
#include "src/modular/lib/modular_config/modular_config.h"
#include "src/modular/lib/modular_config/modular_config_constants.h"
#include "src/modular/lib/modular_test_harness/cpp/fake_settings_intl.h"

constexpr char kBasemgrUrl[] = "fuchsia-pkg://fuchsia.com/basemgr#meta/basemgr.cmx";

class BasemgrImplTest : public sys::testing::TestWithEnvironment {
 public:
  BasemgrImplTest() {}

  void SetUp() override {
    // Setup an enclosing environment that provides basemgr with a mock of the cobalt logger
    // factory.
    auto env_services = CreateServices();
    env_services->AddServiceWithLaunchInfo(
        fuchsia::sys::LaunchInfo{.url =
                                     "fuchsia-pkg://fuchsia.com/mock_cobalt#meta/mock_cobalt.cmx"},
        fuchsia::cobalt::LoggerFactory::Name_);

    env_services->AddServiceWithLaunchInfo(
        fuchsia::sys::LaunchInfo{.url = "fuchsia-pkg://fuchsia.com/device_settings_manager#meta/"
                                        "device_settings_manager.cmx"},
        fuchsia::devicesettings::DeviceSettingsManager::Name_);

    settings_ = modular_testing::FakeSettingsIntl::CreateWithDefaultOptions();
    env_services->AddService(settings_->GetHandler(), fuchsia::settings::Intl::Name_);

    env_ = CreateNewEnclosingEnvironment("basemgr_impl_unittest_env", std::move(env_services),
                                         {.inherit_parent_services = true});
    WaitForEnclosingEnvToStart(env_.get());
  }

  std::unique_ptr<vfs::PseudoDir> CreateConfigPseudoDir(std::string config_str) {
    auto dir = std::make_unique<vfs::PseudoDir>();
    dir->AddEntry(modular_config::kStartupConfigFilePath,
                  std::make_unique<vfs::PseudoFile>(
                      config_str.length(), [config_str = std::move(config_str)](
                                               std::vector<uint8_t>* out, size_t /*unused*/) {
                        std::copy(config_str.begin(), config_str.end(), std::back_inserter(*out));
                        return ZX_OK;
                      }));
    return dir;
  }

  std::string GetTestConfig() {
    return R"config(
      {
      "basemgr": {
        "enable_cobalt":true,
        "use_session_shell_for_story_shell_factory":false,
        "base_shell": {
          "url":"fuchsia-pkg://fuchsia.com/auto_login_base_shell#meta/auto_login_base_shell.cmx",
          "keep_alive_after_login":false,
          "args":[]
        },
        "session_shells": [
          {
            "name":"fuchsia-pkg://fuchsia.com/modular_test_harness#meta/test_session_shell.cmx",
            "url":"fuchsia-pkg://fuchsia.com/modular_test_harness#meta/test_session_shell.cmx",
            "args":[]
          }
        ]
      },
      "sessionmgr": {
        "enable_cobalt":true,
        "startup_agents":[],
        "session_agents":[],
        "restart_session_on_agent_crash":[],
        "component_args":[],
        "agent_service_index":[]
      }
    }
    )config";
  }

  void LaunchBasemgr() {
    std::string config_str = GetTestConfig();

    // Create the pseudo directory with our config "file"
    auto config_dir = CreateConfigPseudoDir(config_str);
    fidl::InterfaceHandle<fuchsia::io::Directory> config_dir_handle;
    config_dir->Serve(fuchsia::io::OPEN_RIGHT_READABLE,
                      config_dir_handle.NewRequest().TakeChannel());

    zx::channel svc_request;
    auto svc_dir = sys::ServiceDirectory::CreateWithRequest(&svc_request);
    FX_CHECK(svc_request.is_valid());

    zx_status_t status = svc_dir->Connect("fuchsia.process.lifecycle.Lifecycle",
                                          process_lifecycle_.NewRequest().TakeChannel());
    FX_CHECK(ZX_OK == status);

    fuchsia::sys::LaunchInfo launch_info;
    launch_info.url = kBasemgrUrl;
    launch_info.flat_namespace = fuchsia::sys::FlatNamespace::New();
    launch_info.flat_namespace->paths.push_back(modular_config::kOverriddenConfigDir);
    launch_info.flat_namespace->directories.push_back(config_dir_handle.TakeChannel());
    launch_info.directory_request = std::move(svc_request);

    bool on_directory_ready = false;
    controller_.events().OnDirectoryReady = [&] { on_directory_ready = true; };

    env_->CreateComponent(std::move(launch_info), controller_.NewRequest());

    RunLoopUntil([&] { return on_directory_ready; });
  }

  std::unique_ptr<sys::testing::EnclosingEnvironment> env_;
  std::unique_ptr<modular_testing::FakeSettingsIntl> settings_;
  fuchsia::sys::ComponentControllerPtr controller_;
  fuchsia::process::lifecycle::LifecyclePtr process_lifecycle_;
};

TEST_F(BasemgrImplTest, BasemgrImplGracefulShutdown) {
  LaunchBasemgr();

  bool is_terminated = false;
  controller_.events().OnTerminated = [&](int64_t return_code,
                                          fuchsia::sys::TerminationReason reason) {
    EXPECT_EQ(EXIT_SUCCESS, return_code);
    EXPECT_EQ(fuchsia::sys::TerminationReason::EXITED, reason);
    is_terminated = true;
  };

  process_lifecycle_->Stop();

  RunLoopUntil([&]() { return is_terminated; });
}