// Copyright 2016 The Fuchsia Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ddk/protocol/tpm.h>
#include <errno.h>
#include <fcntl.h>
#include <magenta/syscalls.h>
#include <magenta/types.h>
#include <mxio/util.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* prog_name;

void print_usage(void) {
    printf("Usage:\n");
    printf("\n");
    printf("%s save\n", prog_name);
    printf("save: Issue a TPM_SaveState command.\n");
}

int cmd_save_state(int fd, int argc, const char** argv) {
    int ret = mxio_ioctl(fd, TPM_IOCTL_SAVE_STATE, NULL, 0, NULL, 0);
    if (ret < 0) {
        printf("Error when saving state: (%d)\n", ret);
        return 1;
    }

    return 0;
}

int main(int argc, const char** argv) {
    if (argc < 1)
        return 1;

    prog_name = argv[0];

    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char* cmd = argv[1];

    argc -= 2;
    argv += 2;

    int fd = open("/dev/tpm", O_RDWR);
    if (fd < 0) {
        printf("Error opening TPM device.\n");
        return 1;
    }

    if (!strcmp("save", cmd)) {
        return cmd_save_state(fd, argc, argv);
    } else {
        printf("Unrecognized command %s.\n", cmd);
        print_usage();
        return 1;
    }

    printf("We should never get here!.\n");
    return 1;
}
