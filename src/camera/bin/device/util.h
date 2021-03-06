// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_CAMERA_BIN_DEVICE_UTIL_H_
#define SRC_CAMERA_BIN_DEVICE_UTIL_H_

#include <fuchsia/camera2/hal/cpp/fidl.h>
#include <fuchsia/camera3/cpp/fidl.h>
#include <lib/async/cpp/task.h>
#include <lib/async/cpp/wait.h>
#include <lib/async/default.h>
#include <lib/fidl/cpp/interface_ptr.h>
#include <lib/fit/result.h>
#include <lib/syslog/cpp/macros.h>
#include <zircon/status.h>
#include <zircon/types.h>

// Safely unbinds a client connection, doing so on the connection's thread if it differs from the
// caller's thread.
template <class T>
inline void Unbind(fidl::InterfacePtr<T>& p) {
  if (!p) {
    return;
  }

  if (p.dispatcher() == async_get_default_dispatcher()) {
    p.Unbind();
    return;
  }

  async::PostTask(p.dispatcher(), [&]() { p.Unbind(); });
}

// Converts a camera2.hal.Config to a camera3.Configuration
inline fit::result<fuchsia::camera3::Configuration2, zx_status_t> Convert(
    const fuchsia::camera2::hal::Config& config) {
  if (config.stream_configs.empty()) {
    FX_LOGS(ERROR) << "Config reported no streams.";
    return fit::error(ZX_ERR_INTERNAL);
  }
  std::vector<fuchsia::camera3::StreamProperties2> streams;
  for (const auto& stream_config : config.stream_configs) {
    if (stream_config.image_formats.empty()) {
      FX_LOGS(ERROR) << "Stream reported no image formats.";
      return fit::error(ZX_ERR_INTERNAL);
    }
    fuchsia::camera3::StreamProperties2 stream_properties;
    stream_properties.set_frame_rate(
        {.numerator = stream_config.frame_rate.frames_per_sec_numerator,
         .denominator = stream_config.frame_rate.frames_per_sec_denominator});
    stream_properties.set_image_format(stream_config.image_formats[0]);
    // TODO(fxbug.dev/50908): Detect ROI support during initialization.
    stream_properties.set_supports_crop_region(true);
    std::vector<fuchsia::math::Size> supported_resolutions;
    for (const auto& format : stream_config.image_formats) {
      supported_resolutions.push_back({.width = static_cast<int32_t>(format.coded_width),
                                       .height = static_cast<int32_t>(format.coded_height)});
    }
    stream_properties.set_supported_resolutions(std::move(supported_resolutions));
    streams.push_back(std::move(stream_properties));
  }
  fuchsia::camera3::Configuration2 ret;
  ret.set_streams(std::move(streams));
  return fit::ok(std::move(ret));
}

// Converts a valid camera3.StreamProperties2 to a camera3.StreamProperties, dropping the
// |supported_resolutions| field.
inline fuchsia::camera3::StreamProperties Convert(
    const fuchsia::camera3::StreamProperties2& properties) {
  return {.image_format = properties.image_format(),
          .frame_rate = properties.frame_rate(),
          .supports_crop_region = properties.supports_crop_region()};
}

// Represents the mute state of a device as defined by the Device.WatchMuteState API.
struct MuteState {
  bool software_muted = false;
  bool hardware_muted = false;
  // Returns true iff any mute is active.
  bool muted() const { return software_muted || hardware_muted; }
  bool operator==(const MuteState& other) const {
    return other.software_muted == software_muted && other.hardware_muted == hardware_muted;
  }
};

// Wraps an async::Wait and frame release fence such that object lifetime requirements are enforced
// automatically, namely that the waited-upon object must outlive the wait object.
class FrameWaiter {
 public:
  FrameWaiter(async_dispatcher_t* dispatcher, zx::eventpair fence, fit::closure signaled)
      : fence_(std::move(fence)),
        wait_(fence_.get(), ZX_EVENTPAIR_PEER_CLOSED, 0,
              fit::bind_member(this, &FrameWaiter::Handler)),
        signaled_(std::move(signaled)) {
    wait_.Begin(dispatcher);
  }
  ~FrameWaiter() {
    wait_.Cancel();
    signaled_ = nullptr;
    fence_.reset();
  }

 private:
  void Handler(async_dispatcher_t* dispatcher, async::Wait* wait, zx_status_t status,
               const zx_packet_signal_t* signal) {
    if (status != ZX_OK) {
      return;
    }
    // |signaled_| may delete |this|, so move it to a local before calling it. This ensures captures
    // are persisted for the duration of the callback.
    auto signaled = std::move(signaled_);
    signaled();
  }
  zx::eventpair fence_;
  async::Wait wait_;
  fit::closure signaled_;
};

#endif  // SRC_CAMERA_BIN_DEVICE_UTIL_H_
