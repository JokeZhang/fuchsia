// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/connectivity/bluetooth/core/bt-host/fidl/low_energy_central_server.h"

#include <cstddef>

#include <gmock/gmock.h>

#include "adapter_test_fixture.h"
#include "fuchsia/bluetooth/gatt/cpp/fidl.h"
#include "fuchsia/bluetooth/le/cpp/fidl.h"
#include "lib/fidl/cpp/interface_request.h"
#include "src/connectivity/bluetooth/core/bt-host/common/status.h"
#include "src/connectivity/bluetooth/core/bt-host/fidl/helpers.h"
#include "src/connectivity/bluetooth/core/bt-host/sm/types.h"
#include "src/connectivity/bluetooth/core/bt-host/testing/fake_controller.h"
#include "src/connectivity/bluetooth/core/bt-host/testing/fake_peer.h"

namespace bthost {
namespace {

namespace fble = fuchsia::bluetooth::le;
const bt::DeviceAddress kTestAddr(bt::DeviceAddress::Type::kLEPublic, {0x01, 0, 0, 0, 0, 0});
const size_t kLEMaxNumPackets = 10;
const bt::hci::DataBufferInfo kLEDataBufferInfo(bt::hci::kMaxACLPayloadSize, kLEMaxNumPackets);

using TestingBase = bthost::testing::AdapterTestFixture;

class FIDL_LowEnergyCentralServerTest : public TestingBase {
 public:
  FIDL_LowEnergyCentralServerTest() = default;
  ~FIDL_LowEnergyCentralServerTest() override = default;

  void SetUp() override {
    AdapterTestFixture::SetUp();

    // Create a LowEnergyCentralServer and bind it to a local client.
    fidl::InterfaceHandle<fble::Central> handle;
    gatt_host_ = std::make_unique<GattHost>(take_gatt());
    server_ = std::make_unique<LowEnergyCentralServer>(adapter(), handle.NewRequest(),
                                                       gatt_host_->AsWeakPtr());
    proxy_.Bind(std::move(handle));

    bt::testing::FakeController::Settings settings;
    settings.ApplyLegacyLEConfig();
    test_device()->set_settings(settings);
  }

  void TearDown() override {
    RunLoopUntilIdle();

    proxy_ = nullptr;
    server_ = nullptr;
    gatt_host_ = nullptr;

    RunLoopUntilIdle();
    AdapterTestFixture::TearDown();
  }

 protected:
  LowEnergyCentralServer* server() const { return server_.get(); }
  fuchsia::bluetooth::le::Central* central_proxy() const { return proxy_.get(); }

 private:
  std::unique_ptr<LowEnergyCentralServer> server_;
  fble::CentralPtr proxy_;
  std::unique_ptr<GattHost> gatt_host_;

  DISALLOW_COPY_AND_ASSIGN_ALLOW_MOVE(FIDL_LowEnergyCentralServerTest);
};

// Tests that connecting to a peripheral with ConnectionOptions.bondable_mode unset results in a
// bondable connection ref being stored in LowEnergyConnectionManager
TEST_F(FIDL_LowEnergyCentralServerTest, ConnectDefaultResultsBondableConnectionRef) {
  auto* const peer = adapter()->peer_cache()->NewPeer(kTestAddr, /*connectable=*/true);
  ASSERT_TRUE(peer);

  test_device()->AddPeer(std::make_unique<bt::testing::FakePeer>(kTestAddr));

  fble::ConnectionOptions options;

  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();

  auto status =
      fidl_helpers::NewFidlError(fuchsia::bluetooth::ErrorCode::BAD_STATE, "this should change");
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    ASSERT_EQ(cb_status.error, nullptr);
    status = std::move(cb_status);
  };
  central_proxy()->ConnectPeripheral(peer->identifier().ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  ASSERT_FALSE(server()->FindConnectionForTesting(peer->identifier()));
  RunLoopUntilIdle();
  auto conn_ref = server()->FindConnectionForTesting(peer->identifier());
  ASSERT_EQ(status.error, nullptr);
  ASSERT_TRUE(conn_ref.has_value());
  ASSERT_TRUE(conn_ref.value());
  ASSERT_EQ(conn_ref.value()->bondable_mode(), bt::sm::BondableMode::Bondable);
}

// Tests that setting ConnectionOptions.bondable_mode to true and connecting to a peer in bondable
// mode results in a bondable connection ref being stored in LowEnergyConnectionManager
TEST_F(FIDL_LowEnergyCentralServerTest, ConnectBondableResultsBondableConnectionRef) {
  auto* const peer = adapter()->peer_cache()->NewPeer(kTestAddr, /*connectable=*/true);
  ASSERT_TRUE(peer);

  test_device()->AddPeer(std::make_unique<bt::testing::FakePeer>(kTestAddr));

  fble::ConnectionOptions options;
  options.set_bondable_mode(true);

  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();

  auto status =
      fidl_helpers::NewFidlError(fuchsia::bluetooth::ErrorCode::BAD_STATE, "this should change");
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    ASSERT_EQ(cb_status.error, nullptr);
    status = std::move(cb_status);
  };
  central_proxy()->ConnectPeripheral(peer->identifier().ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  ASSERT_FALSE(server()->FindConnectionForTesting(peer->identifier()));
  RunLoopUntilIdle();
  auto conn_ref = server()->FindConnectionForTesting(peer->identifier());
  ASSERT_EQ(status.error, nullptr);
  ASSERT_TRUE(conn_ref.has_value());
  ASSERT_TRUE(conn_ref.value());
  ASSERT_EQ(conn_ref.value()->bondable_mode(), bt::sm::BondableMode::Bondable);
}

// Tests that setting ConnectionOptions.bondable_mode to false and connecting to a peer results in
// a non-bondable connection ref being stored in LowEnergyConnectionManager.
TEST_F(FIDL_LowEnergyCentralServerTest, ConnectNonBondableResultsNonBondableConnectionRef) {
  auto* const peer = adapter()->peer_cache()->NewPeer(kTestAddr, /*connectable=*/true);
  ASSERT_TRUE(peer);

  test_device()->AddPeer(std::make_unique<bt::testing::FakePeer>(kTestAddr));

  fble::ConnectionOptions options;
  options.set_bondable_mode(false);

  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();

  auto status =
      fidl_helpers::NewFidlError(fuchsia::bluetooth::ErrorCode::BAD_STATE, "this should change");
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    ASSERT_EQ(cb_status.error, nullptr);
    status = std::move(cb_status);
  };
  central_proxy()->ConnectPeripheral(peer->identifier().ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  ASSERT_FALSE(server()->FindConnectionForTesting(peer->identifier()));
  RunLoopUntilIdle();
  auto conn_ref = server()->FindConnectionForTesting(peer->identifier());
  ASSERT_EQ(status.error, nullptr);
  ASSERT_TRUE(conn_ref.has_value());
  ASSERT_TRUE(conn_ref.value());
  ASSERT_EQ(conn_ref.value()->bondable_mode(), bt::sm::BondableMode::NonBondable);
}

TEST_F(FIDL_LowEnergyCentralServerTest, DisconnectUnconnectedPeripheralReturnsSuccess) {
  auto status =
      fidl_helpers::NewFidlError(fuchsia::bluetooth::ErrorCode::BAD_STATE, "this should change");
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    status = std::move(cb_status);
  };
  central_proxy()->DisconnectPeripheral(bt::PeerId(1).ToString(), std::move(callback));
  RunLoopUntilIdle();
  EXPECT_EQ(status.error, nullptr);
}

TEST_F(FIDL_LowEnergyCentralServerTest, FailedConnectionCleanedUp) {
  auto* const peer = adapter()->peer_cache()->NewPeer(kTestAddr, /*connectable=*/true);
  ASSERT_TRUE(peer);

  test_device()->AddPeer(std::make_unique<bt::testing::FakePeer>(kTestAddr));

  fble::ConnectionOptions options;

  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();

  fuchsia::bluetooth::Status status;
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    status = std::move(cb_status);
  };

  test_device()->SetDefaultCommandStatus(bt::hci::kReadRemoteVersionInfo,
                                         bt::hci::StatusCode::kConnectionFailedToBeEstablished);

  ASSERT_FALSE(server()->FindConnectionForTesting(peer->identifier()).has_value());
  central_proxy()->ConnectPeripheral(peer->identifier().ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  RunLoopUntilIdle();
  auto conn = server()->FindConnectionForTesting(peer->identifier());
  EXPECT_NE(status.error, nullptr);
  EXPECT_FALSE(conn.has_value());
}

TEST_F(FIDL_LowEnergyCentralServerTest, ConnectPeripheralAlreadyConnectedInLecm) {
  auto* const peer = adapter()->peer_cache()->NewPeer(kTestAddr, /*connectable=*/true);
  test_device()->AddPeer(std::make_unique<bt::testing::FakePeer>(kTestAddr));

  bt::gap::LowEnergyConnectionRefPtr le_conn;
  adapter()->le_connection_manager()->Connect(peer->identifier(),
                                              [&le_conn](auto status, auto cb_conn) {
                                                ASSERT_TRUE(status);
                                                le_conn = std::move(cb_conn);
                                              });
  RunLoopUntilIdle();
  ASSERT_TRUE(le_conn);
  ASSERT_FALSE(server()->FindConnectionForTesting(peer->identifier()).has_value());

  fuchsia::bluetooth::Status status;
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    status = std::move(cb_status);
  };

  fble::ConnectionOptions options;
  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();
  central_proxy()->ConnectPeripheral(peer->identifier().ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  RunLoopUntilIdle();
  EXPECT_EQ(status.error, nullptr);
  auto server_conn = server()->FindConnectionForTesting(peer->identifier());
  ASSERT_TRUE(server_conn.has_value());
  EXPECT_NE(server_conn.value(), nullptr);
}

TEST_F(FIDL_LowEnergyCentralServerTest, ConnectPeripheralUnknownPeer) {
  fuchsia::bluetooth::Status status;
  auto callback = [&status](::fuchsia::bluetooth::Status cb_status) {
    status = std::move(cb_status);
  };

  const bt::PeerId peer_id(1);

  fble::ConnectionOptions options;
  fidl::InterfaceHandle<fuchsia::bluetooth::gatt::Client> gatt_client;
  fidl::InterfaceRequest<fuchsia::bluetooth::gatt::Client> gatt_client_req =
      gatt_client.NewRequest();
  central_proxy()->ConnectPeripheral(peer_id.ToString(), std::move(options),
                                     std::move(gatt_client_req), callback);
  RunLoopUntilIdle();
  ASSERT_TRUE(status.error);
  EXPECT_EQ(status.error->error_code, fuchsia::bluetooth::ErrorCode::NOT_FOUND);
  auto server_conn = server()->FindConnectionForTesting(peer_id);
  EXPECT_FALSE(server_conn.has_value());
}

}  // namespace
}  // namespace bthost
