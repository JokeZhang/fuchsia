// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/simple-codec/simple-codec-helper.h>

#include <ddk/debug.h>

namespace audio {

bool IsDaiFormatSupported(const DaiFormat& format,
                          const std::vector<DaiSupportedFormats>& supported) {
  for (size_t i = 0; i < supported.size(); ++i) {
    if (IsDaiFormatSupported(format, supported[i])) {
      return true;
    }
  }
  return false;
}

bool IsDaiFormatSupported(const DaiFormat& format, const DaiSupportedFormats& supported) {
  const sample_format_t sample_format = format.sample_format;
  const frame_format_t frame_format = format.frame_format;
  const uint32_t frame_rate = format.frame_rate;
  const uint8_t bits_per_sample = format.bits_per_sample;
  const uint8_t bits_per_slot = format.bits_per_slot;
  size_t i = 0;
  for (i = 0; i < supported.number_of_channels.size() &&
              supported.number_of_channels[i] != format.number_of_channels;
       ++i) {
  }
  if (i == supported.number_of_channels.size()) {
    zxlogf(INFO, "SimpleCodec did not find number of channels %u", format.number_of_channels);
    return false;
  }

  for (i = 0; i < supported.sample_formats.size() && supported.sample_formats[i] != sample_format;
       ++i) {
  }
  if (i == supported.sample_formats.size()) {
    zxlogf(INFO, "SimpleCodec did not find sample format %d", static_cast<int>(sample_format));
    return false;
  }

  size_t j = 0;
  for (i = 0; i < supported.frame_formats.size(); ++i) {
    if (supported.frame_formats[i] == frame_format) {
      if (frame_format != FRAME_FORMAT_CUSTOM) {
        break;
      } else {
        if (j < supported.frame_formats_custom.size()) {
          auto& this_supported = supported.frame_formats_custom[j];
          auto& this_format = format.frame_format_custom;
          if (this_supported.left_justified == this_format.left_justified &&
              this_supported.sclk_on_raising == this_format.sclk_on_raising &&
              this_supported.frame_sync_sclks_offset == this_format.frame_sync_sclks_offset &&
              this_supported.frame_sync_size == this_format.frame_sync_size) {
            break;
          }
        }
        j++;
      }
    }
  }
  if (i == supported.frame_formats.size()) {
    zxlogf(INFO, "SimpleCodec did not find frame format %d", static_cast<int>(frame_format));
    return false;
  }

  for (i = 0; i < supported.frame_rates.size() && supported.frame_rates[i] != frame_rate; ++i) {
  }
  if (i == supported.frame_rates.size()) {
    zxlogf(INFO, "SimpleCodec did not find sample rate %u", frame_rate);
    return false;
  }

  for (i = 0;
       i < supported.bits_per_sample.size() && supported.bits_per_sample[i] != bits_per_sample;
       ++i) {
  }
  if (i == supported.bits_per_sample.size()) {
    zxlogf(INFO, "SimpleCodec did not find bits per sample %u", bits_per_sample);
    return false;
  }

  for (i = 0; i < supported.bits_per_slot.size() && supported.bits_per_slot[i] != bits_per_slot;
       ++i) {
  }
  if (i == supported.bits_per_slot.size()) {
    zxlogf(INFO, "SimpleCodec did not find wanted bits per slot %u", bits_per_slot);
    return false;
  }

  if (bits_per_sample > bits_per_slot) {
    zxlogf(INFO, "SimpleCodec found bits per sample (%u) too big for the bits per slot (%u)",
           bits_per_sample, bits_per_slot);
    return false;
  }

  zxlogf(INFO, "SimpleCodec found a supported format");
  return true;
}

}  // namespace audio
