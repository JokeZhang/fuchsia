// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_ZBITL_FD_H_
#define LIB_ZBITL_FD_H_

#include <sys/types.h>  // off_t

#include <fbl/unique_fd.h>

#include "storage_traits.h"

namespace zbitl {

/// zbitl::View<fbl::unique_fd> is a move-only type that owns the fd.
template <>
class StorageTraits<fbl::unique_fd> {
 public:
  /// File I/O errors are represented by an errno value.
  using error_type = int;

  /// Offset into file where the ZBI item payload begins.
  using payload_type = off_t;

  static fitx::result<error_type, uint32_t> Capacity(const fbl::unique_fd&);

  static fitx::result<error_type, zbi_header_t> Header(const fbl::unique_fd&, uint32_t offset);

  static fitx::result<error_type, payload_type> Payload(const fbl::unique_fd&, uint32_t offset,
                                                        uint32_t length) {
    return fitx::ok(offset);
  }

  template <typename Callback>
  static auto Read(const fbl::unique_fd& zbi, payload_type payload, uint32_t length,
                   Callback&& callback)
      -> fitx::result<error_type, decltype(callback(ByteView{}))> {
    decltype(callback(ByteView{})) result = fitx::ok();
    auto cb = [&](ByteView chunk) -> bool {
      result = callback(chunk);
      return result.is_ok();
    };
    using CbType = decltype(cb);
    if (auto read_error = DoRead(
            zbi, payload, length,
            [](void* cb, ByteView chunk) { return (*static_cast<CbType*>(cb))(chunk); }, &cb);
        read_error.is_error()) {
      return fitx::error{read_error.error_value()};
    } else {
      return fitx::ok(result);
    }
  }

  static fitx::result<error_type> Write(const fbl::unique_fd&, uint32_t offset, ByteView data);

 private:
  static fitx::result<error_type> DoRead(const fbl::unique_fd& zbi, off_t offset, uint32_t length,
                                         bool (*)(void*, ByteView), void*);
};

/// zbitl::View<const fbl::unique_fd&> is an unmovable, uncopyable type that
/// refers to a fbl::unique_fd it does not own.
template <>
struct StorageTraits<const fbl::unique_fd&> : public StorageTraits<fbl::unique_fd> {};

}  // namespace zbitl

#endif  // LIB_ZBITL_FD_H_
