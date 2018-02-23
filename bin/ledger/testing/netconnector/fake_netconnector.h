// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PERIDOT_BIN_LEDGER_TESTING_NETCONNECTOR_NETCONNECTOR_H_
#define PERIDOT_BIN_LEDGER_TESTING_NETCONNECTOR_NETCONNECTOR_H_

#include "lib/app/cpp/service_provider_impl.h"
#include "lib/fxl/macros.h"
#include "lib/netconnector/fidl/netconnector.fidl.h"

namespace ledger {

// FakeNetConnector implements NetConnector. It acts as the singleton
// NetConnector for a (virtual) host.
class FakeNetConnector : public netconnector::NetConnector {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    // Returns the list of known devices. See NetConnector::GetKnownDeviceNames
    // for more details.
    virtual void GetDevicesNames(
        uint64_t last_version,
        std::function<void(uint64_t, f1dl::Array<f1dl::String>)> callback) = 0;

    // Connects to the ServiceProvider from host |device_name|.
    virtual void ConnectToServiceProvider(
        std::string device_name,
        f1dl::InterfaceRequest<app::ServiceProvider> request) = 0;
  };

  FakeNetConnector(Delegate* delegate);
  ~FakeNetConnector() override {}

  // Connects to the service provider of this (virtual) host
  void ConnectToServiceProvider(
      f1dl::InterfaceRequest<app::ServiceProvider> request);

 private:
  // NetConnector implementation:
  void RegisterServiceProvider(
      const f1dl::String& name,
      f1dl::InterfaceHandle<app::ServiceProvider> service_provider) override;
  void GetDeviceServiceProvider(
      const f1dl::String& device_name,
      f1dl::InterfaceRequest<app::ServiceProvider> service_provider) override;
  void GetKnownDeviceNames(
      uint64_t version_last_seen,
      const GetKnownDeviceNamesCallback& callback) override;

  app::ServiceProviderImpl service_provider_impl_;
  Delegate* const delegate_;

  FXL_DISALLOW_COPY_AND_ASSIGN(FakeNetConnector);
};

}  // namespace ledger

#endif  // PERIDOT_BIN_LEDGER_TESTING_NETCONNECTOR_NETCONNECTOR_H_
