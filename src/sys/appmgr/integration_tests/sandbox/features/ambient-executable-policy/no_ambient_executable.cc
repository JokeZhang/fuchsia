// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <zircon/status.h>
#include <zircon/syscalls.h>

#include "src/sys/appmgr/integration_tests/sandbox/namespace_test.h"

TEST_F(NamespaceTest, NoAmbientExecutable) {
  zx::vmo vmo, vmo2, vmo3;

  // allocate an object
  ASSERT_EQ(ZX_OK, zx_vmo_create(PAGE_SIZE, 0, vmo.reset_and_get_address()));

  // set-exec with an invalid VMEX resource handle
  ASSERT_EQ(ZX_OK, zx_handle_duplicate(vmo.get(), ZX_RIGHT_READ, vmo2.reset_and_get_address()));
  ASSERT_EQ(ZX_ERR_ACCESS_DENIED, zx_vmo_replace_as_executable(vmo2.get(), ZX_HANDLE_INVALID,
                                                               vmo3.reset_and_get_address()));
  EXPECT_EQ(ZX_HANDLE_INVALID, vmo3);
}
