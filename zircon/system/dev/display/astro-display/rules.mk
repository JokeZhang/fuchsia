# Copyright 2018 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := driver

MODULE_SRCS += \
    $(LOCAL_DIR)/bind.c \
    $(LOCAL_DIR)/osd.cpp \
    $(LOCAL_DIR)/astro-clock.cpp \
    $(LOCAL_DIR)/dw-mipi-dsi.cpp \
    $(LOCAL_DIR)/aml-mipi-phy.cpp \
    $(LOCAL_DIR)/aml-dsi-host.cpp \
    $(LOCAL_DIR)/vpu.cpp \
    $(LOCAL_DIR)/lcd.cpp \
    $(LOCAL_DIR)/astro-display.cpp \

MODULE_STATIC_LIBS := \
    system/dev/lib/mmio \
    system/ulib/bitmap \
    system/ulib/sync \
    system/ulib/ddk \
    system/ulib/ddktl \
    system/ulib/hwreg \
    system/ulib/fbl \
    system/ulib/fidl \
    system/ulib/zx \
    system/ulib/zxcpp \

MODULE_LIBS := \
    system/ulib/driver \
    system/ulib/zircon \
    system/ulib/c \

MODULE_BANJO_LIBS := \
    system/banjo/ddk-protocol-amlogiccanvas \
    system/banjo/ddk-protocol-display-controller \
    system/banjo/ddk-protocol-gpio \
    system/banjo/ddk-protocol-i2c \
    system/banjo/ddk-protocol-platform-device \
    system/banjo/ddk-protocol-sysmem \

MODULE_FIDL_LIBS := system/fidl/fuchsia-sysmem

include make/module.mk
