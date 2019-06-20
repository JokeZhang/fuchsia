// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "arm-isp-test.h"

#include <ddk/debug.h>
#include <fbl/alloc_checker.h>
#include <fbl/auto_lock.h>
#include <fbl/vector.h>
#include <fuchsia/camera/test/c/fidl.h>
#include <zircon/fidl.h>

#include "arm-isp.h"
#include "src/camera/drivers/test_utils/fake-buffer-collection.h"

namespace camera {

zx_status_t ArmIspDeviceTester::Create(ArmIspDevice* isp,
                                       fit::callback<void()>* on_isp_unbind) {
  fbl::AllocChecker ac;
  auto isp_test_device =
      fbl::make_unique_checked<ArmIspDeviceTester>(&ac, isp, isp->zxdev());
  if (!ac.check()) {
    zxlogf(ERROR, "%s: Unable to start ArmIspDeviceTester \n", __func__);
    return ZX_ERR_NO_MEMORY;
  }

  *on_isp_unbind =
      fit::bind_member(isp_test_device.get(), &ArmIspDeviceTester::Disconnect);

  zx_status_t status = isp_test_device->DdkAdd("arm-isp-tester");
  if (status != ZX_OK) {
    zxlogf(ERROR, "Could not create arm-isp-tester device: %d\n", status);
    return status;
  } else {
    zxlogf(INFO, "arm-isp: Added arm-isp-tester device\n");
  }

  // isp_test_device intentionally leaked as it is now held by DevMgr.
  __UNUSED auto ptr = isp_test_device.release();

  return status;
}

// Methods required by the ddk.
void ArmIspDeviceTester::DdkRelease() { delete this; }

void ArmIspDeviceTester::DdkUnbind() { DdkRemove(); }

void ArmIspDeviceTester::Disconnect() {
  fbl::AutoLock guard(&isp_lock_);
  isp_ = nullptr;
}

zx_status_t ArmIspDeviceTester::DdkMessage(fidl_msg_t* msg, fidl_txn_t* txn) {
  return fuchsia_camera_test_IspTester_dispatch(this, txn, msg,
                                                &isp_tester_ops);
}
struct FrameReadyReceiver {
  fbl::Vector<uint32_t> ready_ids;
  void FrameReady(uint32_t buffer_id) { ready_ids.push_back(buffer_id); }
  output_stream_callback GetCallback() {
    output_stream_callback cb;
    cb.ctx = this;
    cb.frame_ready = [](void* ctx, uint32_t buffer_id) {
      reinterpret_cast<FrameReadyReceiver*>(ctx)->FrameReady(buffer_id);
    };
    return cb;
  }
};

// These macros provide a similar style to our normal zxtest/gtests,
// but it actually do something much different under the hood.
// Instead of registering with a gtest framework, the results are summed
// by a local variable called |reports|, which is later passed back over
// a fidl interface to the test binary.
// TODO(CAM-82): Use these macros more widely in this class, and add a
// TEST() macro to deal with the report management.
#define ASSERT_OK(expr, msg)              \
  report.test_count++;                    \
  if ((expr) == ZX_OK) {                  \
    report.success_count++;               \
  } else {                                \
    report.failure_count++;               \
    zxlogf(ERROR, "[FAILURE] %s\n", msg); \
    return;                               \
  }

#define EXPECT_OK(expr, msg)              \
  report.test_count++;                    \
  if ((expr) == ZX_OK) {                  \
    report.success_count++;               \
  } else {                                \
    report.failure_count++;               \
    zxlogf(ERROR, "[FAILURE] %s\n", msg); \
  }

#define EXPECT_NOT_OK(expr, msg)          \
  report.test_count++;                    \
  if ((expr) != ZX_OK) {                  \
    report.success_count++;               \
  } else {                                \
    report.failure_count++;               \
    zxlogf(ERROR, "[FAILURE] %s\n", msg); \
  }

#define EXPECT_EQ(expr1, expr2, msg)      \
  report.test_count++;                    \
  if ((expr1) == (expr2)) {               \
    report.success_count++;               \
  } else {                                \
    report.failure_count++;               \
    zxlogf(ERROR, "[FAILURE] %s\n", msg); \
  }

#define ASSERT_EQ(expr1, expr2, msg)      \
  report.test_count++;                    \
  if ((expr1) == (expr2)) {               \
    report.success_count++;               \
  } else {                                \
    report.failure_count++;               \
    zxlogf(ERROR, "[FAILURE] %s\n", msg); \
    return;                               \
  }

void ArmIspDeviceTester::TestWriteRegister(
    fuchsia_camera_test_TestReport& report) {
  // We'll enable then disable the global debug register:
  fbl::AutoLock guard(&isp_lock_);
  uint32_t offset =
      IspGlobalDbg::Get().addr() / 4;  // divide by 4 to get the word address.
  IspGlobalDbg::Get()
      .ReadFrom(&(isp_->isp_mmio_))
      .set_mode_en(1)
      .WriteTo(&(isp_->isp_mmio_));
  ArmIspRegisterDump after_enable = isp_->DumpRegisters();
  EXPECT_EQ(after_enable.global_config[offset], 1,
            "Global debug was not enabled!");
  IspGlobalDbg::Get()
      .ReadFrom(&(isp_->isp_mmio_))
      .set_mode_en(0)
      .WriteTo(&(isp_->isp_mmio_));
  ArmIspRegisterDump after_disable = isp_->DumpRegisters();
  EXPECT_EQ(after_disable.global_config[offset], 0,
            "Global debug was not disabled!");
}

void ArmIspDeviceTester::TestConnectStream(
    fuchsia_camera_test_TestReport& report) {
  constexpr uint32_t kWidth = 1080;
  constexpr uint32_t kHeight = 764;
  constexpr uint32_t kNumberOfBuffers = 8;
  fuchsia_camera_common_FrameRate rate = {.frames_per_sec_numerator = 30,
                                          .frames_per_sec_denominator = 1};
  FrameReadyReceiver receiver;
  output_stream_callback cb = receiver.GetCallback();
  output_stream_protocol out_st;
  output_stream_protocol_ops_t ops;
  out_st.ops = &ops;

  fuchsia_sysmem_BufferCollectionInfo buffer_collection;
  fbl::AutoLock guard(&isp_lock_);
  ASSERT_OK(camera::CreateContiguousBufferCollectionInfo(
                &buffer_collection, isp_->bti_.get(), kWidth, kHeight,
                kNumberOfBuffers),
            "Failed to Create Contiguous buffers");

  EXPECT_OK(
      isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                  STREAM_TYPE_FULL_RESOLUTION, &cb, &out_st),
      "Failed to Create FullRes input stream");

  EXPECT_OK(isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                        STREAM_TYPE_DOWNSCALED, &cb, &out_st),
            "Failed to Create Downscaled input stream");

  EXPECT_EQ(isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                        STREAM_TYPE_SCALAR, &cb, &out_st),
            ZX_ERR_NOT_SUPPORTED,
            "Failed to return ZX_ERR_NOT_SUPPORTED for Scalar stream");

  EXPECT_EQ(isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                        STREAM_TYPE_INVALID, &cb, &out_st),
            ZX_ERR_INVALID_ARGS,
            "Failed to return ZX_ERR_INVALID_ARGS for invalid stream");
}

void ArmIspDeviceTester::TestCallbacks(fuchsia_camera_test_TestReport& report) {
  constexpr uint32_t kWidth = 1920;
  constexpr uint32_t kHeight = 1080;
  constexpr uint32_t kNumberOfBuffers = 8;
  fuchsia_camera_common_FrameRate rate = {.frames_per_sec_numerator = 30,
                                          .frames_per_sec_denominator = 1};
  FrameReadyReceiver full_res_receiver;
  FrameReadyReceiver downscaled_receiver;
  output_stream_callback full_res_cb = full_res_receiver.GetCallback();
  output_stream_protocol full_res_out_st, downscaled_out_st;
  output_stream_protocol_ops_t full_res_ops, downscaled_ops;
  full_res_out_st.ops = &full_res_ops;
  downscaled_out_st.ops = &downscaled_ops;

  fuchsia_sysmem_BufferCollectionInfo buffer_collection;
  fbl::AutoLock guard(&isp_lock_);
  ASSERT_OK(camera::CreateContiguousBufferCollectionInfo(
                &buffer_collection, isp_->bti_.get(), kWidth, kHeight,
                kNumberOfBuffers),
            "Failed to Create Contiguous buffers.");

  EXPECT_OK(isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                        STREAM_TYPE_FULL_RESOLUTION,
                                        &full_res_cb, &full_res_out_st),
            "Failed to Create FullRes input stream.");

  output_stream_callback downscaled_cb = downscaled_receiver.GetCallback();
  EXPECT_OK(isp_->IspCreateOutputStream(&buffer_collection, &rate,
                                        STREAM_TYPE_DOWNSCALED, &downscaled_cb,
                                        &downscaled_out_st),
            "Failed to Create Downscaled input stream.");

  // Try to release a frame before things are started.  Should fail.
  EXPECT_NOT_OK(
      full_res_out_st.ops->release_frame(full_res_out_st.ctx, 0),
      "Unexpected success from releasing un-started full-res stream.");
  EXPECT_NOT_OK(
      downscaled_out_st.ops->release_frame(downscaled_out_st.ctx, 0),
      "Unexpected success from releasing un-started downscaled stream.");

  // Manually cycle dma through one frame, see that callbacks are called
  isp_->full_resolution_dma_->Enable();
  isp_->full_resolution_dma_->OnNewFrame();
  isp_->full_resolution_dma_->OnFrameWritten();
  EXPECT_EQ(full_res_receiver.ready_ids.size(), 1,
            "Full res callbacks not equal to 1.");
  EXPECT_EQ(downscaled_receiver.ready_ids.size(), 0,
            "Downscaled callbacks not equal to 1.");
  isp_->downscaled_dma_->Enable();
  isp_->downscaled_dma_->OnNewFrame();
  isp_->downscaled_dma_->OnFrameWritten();
  ASSERT_EQ(full_res_receiver.ready_ids.size(), 1,
            "Full res callbacks not equal to 1.");
  ASSERT_EQ(downscaled_receiver.ready_ids.size(), 1,
            "Downscaled callbacks not equal to 1.");

  // Release frame should now work
  EXPECT_OK(full_res_out_st.ops->release_frame(full_res_out_st.ctx,
                                               full_res_receiver.ready_ids[0]),
            "Failed to release frame from full-res stream.");
  EXPECT_OK(downscaled_out_st.ops->release_frame(
                downscaled_out_st.ctx, downscaled_receiver.ready_ids[0]),
            "Failed to release frame from downscaled stream.");

  // TODO(CAM-78): Add tests for Start and Stop when the interrupts are hooked
  // up.
}

// DDKMessage Helper Functions.
zx_status_t ArmIspDeviceTester::RunTests(fidl_txn_t* txn) {
  fuchsia_camera_test_TestReport report = {1, 0, 0};
  {
    fbl::AutoLock guard(&isp_lock_);
    if (!isp_) {
      return ZX_ERR_BAD_STATE;
    }
    if (isp_->RunTests() == ZX_OK) {
      report.success_count++;
    } else {
      report.failure_count++;
    }
  }
  TestWriteRegister(report);
  TestConnectStream(report);
  TestCallbacks(report);
  return fuchsia_camera_test_IspTesterRunTests_reply(txn, ZX_OK, &report);
}

}  // namespace camera
