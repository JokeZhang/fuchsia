#ifndef SYSROOT_ERRNO_H_
#define SYSROOT_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/errno.h>

int* __errno_location(void);
#define errno (*__errno_location())

#ifdef _GNU_SOURCE
extern char *program_invocation_short_name, *program_invocation_name;
#endif

#ifdef __cplusplus
}
#endif

#endif  // SYSROOT_ERRNO_H_
