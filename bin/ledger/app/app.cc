// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>
#include <utility>

#include <errno.h>

#include "apps/ledger/services/ledger.fidl.h"
#include "apps/ledger/src/app/ledger_factory_impl.h"
#include "apps/modular/lib/app/application_context.h"
#include "apps/network/services/network_service.fidl.h"
#include "lib/fidl/cpp/bindings/binding_set.h"
#include "lib/ftl/files/directory.h"
#include "lib/ftl/logging.h"
#include "lib/ftl/macros.h"
#include "lib/mtl/tasks/message_loop.h"

namespace ledger {

namespace {

const char kDefaultStoragePath[] = "/data/ledger";

}  // namespace

// App is the main entry point of the Ledger Mojo application.
//
// It is responsible for setting up the LedgerFactory, which connects clients to
// individual ledger instances. It should not however hold long-lived objects
// shared between ledger instances, as we need to be able to put them in
// separate processes when the app becomes multi-instance.
class App {
 public:
  App()
      : application_context_(
            modular::ApplicationContext::CreateFromStartupInfo()) {
    FTL_DCHECK(application_context_);

    storage_path_ = kDefaultStoragePath;

    // TODO(qsr): Look for arguments to setup another storage path.

    if (!files::IsDirectory(storage_path_) &&
        !files::CreateDirectory(storage_path_)) {
      FTL_LOG(ERROR) << "Unable to access " << storage_path_;
      exit(EACCES);
      return;
    }

    factory_impl_.reset(new LedgerFactoryImpl(
        mtl::MessageLoop::GetCurrent()->task_runner(), storage_path_));

    application_context_->outgoing_services()->AddService<LedgerFactory>(
        [this](fidl::InterfaceRequest<LedgerFactory> request) {
          factory_bindings_.AddBinding(factory_impl_.get(), std::move(request));
        });
  }
  ~App() {}

 private:
  std::unique_ptr<modular::ApplicationContext> application_context_;
  std::string storage_path_;
  std::unique_ptr<LedgerFactoryImpl> factory_impl_;
  fidl::BindingSet<LedgerFactory> factory_bindings_;

  FTL_DISALLOW_COPY_AND_ASSIGN(App);
};

}  // namespace ledger

int main(int argc, const char** argv) {
  mtl::MessageLoop loop;

  ledger::App app;

  loop.Run();
  return 0;
}
