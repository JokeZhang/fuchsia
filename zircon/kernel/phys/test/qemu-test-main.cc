// Copyright 2020 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <lib/uart/qemu.h>
#include <stdio.h>
#include <stdlib.h>

#include "../main.h"
#include "test-main.h"

FILE FILE::stdout_;

void PhysMain(void* zbi, arch::EarlyTicks ticks) {
  uart::qemu::KernelDriver<> uart;
  FILE::stdout_ = FILE{&uart};

  // The qemu-phys tests don't use the argument at all.  Pass nullptr to easily
  // distinguish qemu-phys tests from proper ZBI tests in the shared test code.
  int status = TestMain(nullptr, ticks);
  if (status == 0) {
    printf("*** Test succeeded ***\n%s\n", ZBI_TEST_SUCCESS_STRING);
  } else {
    printf("*** Test FAILED: status %d ***\n", status);
  }

  // No way to shut down.
  abort();
}
