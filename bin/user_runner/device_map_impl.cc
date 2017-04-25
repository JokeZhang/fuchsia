// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "apps/modular/src/user_runner/device_map_impl.h"

#include "apps/modular/lib/fidl/array_to_string.h"
#include "apps/modular/lib/fidl/json_xdr.h"
#include "apps/modular/lib/ledger/storage.h"
#include "apps/modular/lib/rapidjson/rapidjson.h"
#include "lib/fidl/cpp/bindings/array.h"
#include "lib/ftl/time/time_point.h"
#include "lib/mtl/vmo/strings.h"

namespace modular {

namespace {

void XdrDeviceData(XdrContext* const xdr, DeviceMapEntryPtr* const data) {
  xdr->Field("name", &data->get()->name);
  xdr->Field("device_id", &data->get()->device_id);
  xdr->Field("profile", &data->get()->profile);
}

void WriteDeviceData(const std::string& device_name,
                     const std::string& device_id,
                     const std::string& profile,
                     ledger::Page* const page) {
  std::string json;
  DeviceMapEntryPtr device = DeviceMapEntry::New();
  device->name = device_name;
  device->device_id = device_id;
  device->profile = profile;
  XdrWrite(&json, &device, XdrDeviceData);
  page->Put(to_array(MakeDeviceKey(device_id)), to_array(json),
            [](ledger::Status) {});
}

}  // namespace

// Asynchronous operations of this service.

class DeviceMapImpl::QueryCall : Operation<fidl::Array<DeviceMapEntryPtr>> {
 public:
  QueryCall(OperationContainer* const container,
            std::shared_ptr<ledger::PageSnapshotPtr> const snapshot,
            ResultCall result_call)
      : Operation(container, std::move(result_call)), snapshot_(snapshot) {
    data_.resize(0);  // never return null
    Ready();
  }

 private:
  void Run() override { GetEntries(nullptr); }

  void GetEntries(fidl::Array<uint8_t> continuation_token) {
    (*snapshot_)
        ->GetEntries(
            to_array(kDeviceKeyPrefix), std::move(continuation_token),
            [this](ledger::Status status, fidl::Array<ledger::EntryPtr> entries,
                   fidl::Array<uint8_t> continuation_token) {
              if (status != ledger::Status::OK &&
                  status != ledger::Status::PARTIAL_RESULT) {
                FTL_LOG(ERROR) << "Ledger status " << status << ".";
                Done(std::move(data_));
                return;
              }

              if (entries.size() == 0) {
                // No existing entries.
                Done(std::move(data_));
                return;
              }

              for (const auto& entry : entries) {
                std::string value;
                if (!mtl::StringFromVmo(entry->value, &value)) {
                  FTL_LOG(ERROR) << "VMO for key " << to_string(entry->key)
                                 << " couldn't be copied.";
                  continue;
                }

                auto device = DeviceMapEntry::New();
                if (!XdrRead(value, &device, XdrDeviceData)) {
                  continue;
                }

                data_.push_back(std::move(device));
              }

              if (status == ledger::Status::PARTIAL_RESULT) {
                GetEntries(std::move(continuation_token));
              } else {
                Done(std::move(data_));
              }
            });
  }

  std::shared_ptr<ledger::PageSnapshotPtr> snapshot_;
  fidl::Array<DeviceMapEntryPtr> data_;
  FTL_DISALLOW_COPY_AND_ASSIGN(QueryCall);
};

DeviceMapImpl::DeviceMapImpl(const std::string& device_name,
                             const std::string& device_id,
                             const std::string& device_profile,
                             ledger::Page* const page)
    : page_(page), page_watcher_binding_(this), page_client_("DeviceMapImpl") {
  page_->GetSnapshot(
      page_client_.NewRequest(), to_array(kDeviceKeyPrefix),
      page_watcher_binding_.NewBinding(), [](ledger::Status status) {
        if (status != ledger::Status::OK) {
          FTL_LOG(ERROR) << "Page.GetSnapshot() status: " << status;
        }
      });

  WriteDeviceData(device_name, device_id, device_profile, page_);
}

DeviceMapImpl::~DeviceMapImpl() = default;

void DeviceMapImpl::Connect(fidl::InterfaceRequest<DeviceMap> request) {
  bindings_.AddBinding(this, std::move(request));
}

void DeviceMapImpl::Query(const QueryCallback& callback) {
  new QueryCall(&operation_queue_, page_client_.page_snapshot(), callback);
}

void DeviceMapImpl::OnChange(ledger::PageChangePtr page,
                             ledger::ResultState result_state,
                             const OnChangeCallback& callback) {
  bool update = false;
  for (auto& entry : page->changes) {
    FTL_LOG(INFO) << "New Device: " << to_string(entry->key);
    update = true;
  }

  // We request a new page snapshot if we see a new device. We have to
  // do this regardless of continuation state, because there might be
  // no keys we listen to in the last continuation.
  if (update) {
    callback(page_client_.NewRequest());
  } else {
    callback(nullptr);
  }
}

}  // namespace modular
