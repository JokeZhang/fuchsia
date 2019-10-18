// WARNING: This file is machine generated by fidlgen.

#include <fuchsia/input/report/llcpp/fidl.h>
#include <memory>

namespace llcpp {

namespace fuchsia {
namespace input {
namespace report {

::llcpp::fuchsia::input::report::SensorReport::Builder SensorReport::Build() {
  return SensorReport::Builder();
}

auto ::llcpp::fuchsia::input::report::SensorReport::Builder::set_values(::fidl::VectorView<int64_t>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::SensorDescriptor::Builder SensorDescriptor::Build() {
  return SensorDescriptor::Builder();
}

auto ::llcpp::fuchsia::input::report::SensorDescriptor::Builder::set_values(::fidl::VectorView<::llcpp::fuchsia::input::report::SensorAxis>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::MouseDescriptor::Builder MouseDescriptor::Build() {
  return MouseDescriptor::Builder();
}

auto ::llcpp::fuchsia::input::report::MouseDescriptor::Builder::set_movement_x(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseDescriptor::Builder::set_movement_y(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseDescriptor::Builder::set_scroll_v(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseDescriptor::Builder::set_scroll_h(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseDescriptor::Builder::set_buttons(::fidl::VectorView<uint8_t>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[5 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 5) {
    max_ordinal_ = 5;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::ContactDescriptor::Builder ContactDescriptor::Build() {
  return ContactDescriptor::Builder();
}

auto ::llcpp::fuchsia::input::report::ContactDescriptor::Builder::set_position_x(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactDescriptor::Builder::set_position_y(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactDescriptor::Builder::set_pressure(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactDescriptor::Builder::set_contact_width(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactDescriptor::Builder::set_contact_height(::llcpp::fuchsia::input::report::Axis* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[5 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 5) {
    max_ordinal_ = 5;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::TouchDescriptor::Builder TouchDescriptor::Build() {
  return TouchDescriptor::Builder();
}

auto ::llcpp::fuchsia::input::report::TouchDescriptor::Builder::set_contacts(::fidl::VectorView<::llcpp::fuchsia::input::report::ContactDescriptor>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::TouchDescriptor::Builder::set_max_contacts(uint32_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::TouchDescriptor::Builder::set_touch_type(::llcpp::fuchsia::input::report::TouchType* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::MouseReport::Builder MouseReport::Build() {
  return MouseReport::Builder();
}

auto ::llcpp::fuchsia::input::report::MouseReport::Builder::set_movement_x(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseReport::Builder::set_movement_y(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseReport::Builder::set_scroll_v(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseReport::Builder::set_scroll_h(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::MouseReport::Builder::set_pressed_buttons(::fidl::VectorView<uint8_t>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[5 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 5) {
    max_ordinal_ = 5;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::DeviceDescriptor::Builder DeviceDescriptor::Build() {
  return DeviceDescriptor::Builder();
}

auto ::llcpp::fuchsia::input::report::DeviceDescriptor::Builder::set_device_info(::llcpp::fuchsia::input::report::DeviceInfo* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::DeviceDescriptor::Builder::set_mouse(::llcpp::fuchsia::input::report::MouseDescriptor* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::DeviceDescriptor::Builder::set_sensor(::llcpp::fuchsia::input::report::SensorDescriptor* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::DeviceDescriptor::Builder::set_touch(::llcpp::fuchsia::input::report::TouchDescriptor* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::ContactReport::Builder ContactReport::Build() {
  return ContactReport::Builder();
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_contact_id(uint32_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_position_x(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_position_y(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_pressure(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_contact_width(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[5 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 5) {
    max_ordinal_ = 5;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::ContactReport::Builder::set_contact_height(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[6 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 6) {
    max_ordinal_ = 6;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::TouchReport::Builder TouchReport::Build() {
  return TouchReport::Builder();
}

auto ::llcpp::fuchsia::input::report::TouchReport::Builder::set_contacts(::fidl::VectorView<::llcpp::fuchsia::input::report::ContactReport>* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

::llcpp::fuchsia::input::report::InputReport::Builder InputReport::Build() {
  return InputReport::Builder();
}

auto ::llcpp::fuchsia::input::report::InputReport::Builder::set_event_time(int64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[1 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 1) {
    max_ordinal_ = 1;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::InputReport::Builder::set_mouse(::llcpp::fuchsia::input::report::MouseReport* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[2 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 2) {
    max_ordinal_ = 2;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::InputReport::Builder::set_trace_id(uint64_t* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[3 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 3) {
    max_ordinal_ = 3;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::InputReport::Builder::set_sensor(::llcpp::fuchsia::input::report::SensorReport* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[4 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 4) {
    max_ordinal_ = 4;
  }
  return std::move(*this);
}

auto ::llcpp::fuchsia::input::report::InputReport::Builder::set_touch(::llcpp::fuchsia::input::report::TouchReport* elem) -> Builder&& {
  ZX_ASSERT(elem);
  envelopes_[5 - 1].data = static_cast<void*>(elem);
  if (max_ordinal_ < 5) {
    max_ordinal_ = 5;
  }
  return std::move(*this);
}

namespace {

[[maybe_unused]]
constexpr uint64_t kInputDevice_GetReportsEvent_Ordinal = 0x49e5124900000000lu;
[[maybe_unused]]
constexpr uint64_t kInputDevice_GetReportsEvent_GenOrdinal = 0x2188910c9bac05aclu;
extern "C" const fidl_type_t fuchsia_input_report_InputDeviceGetReportsEventResponseTable;
[[maybe_unused]]
constexpr uint64_t kInputDevice_GetReports_Ordinal = 0x28a404a100000000lu;
[[maybe_unused]]
constexpr uint64_t kInputDevice_GetReports_GenOrdinal = 0x5ba2344d51c69c2blu;
extern "C" const fidl_type_t fuchsia_input_report_InputDeviceGetReportsResponseTable;
[[maybe_unused]]
constexpr uint64_t kInputDevice_GetDescriptor_Ordinal = 0x5456d97300000000lu;
[[maybe_unused]]
constexpr uint64_t kInputDevice_GetDescriptor_GenOrdinal = 0x3d76420f2ff8ad32lu;
extern "C" const fidl_type_t fuchsia_input_report_InputDeviceGetDescriptorResponseTable;

}  // namespace
template <>
InputDevice::ResultOf::GetReportsEvent_Impl<InputDevice::GetReportsEventResponse>::GetReportsEvent_Impl(zx::unowned_channel _client_end) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetReportsEventRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, GetReportsEventRequest::PrimarySize);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetReportsEventRequest));
  ::fidl::DecodedMessage<GetReportsEventRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      InputDevice::InPlace::GetReportsEvent(std::move(_client_end), Super::response_buffer()));
}

InputDevice::ResultOf::GetReportsEvent InputDevice::SyncClient::GetReportsEvent() {
  return ResultOf::GetReportsEvent(zx::unowned_channel(this->channel_));
}

InputDevice::ResultOf::GetReportsEvent InputDevice::Call::GetReportsEvent(zx::unowned_channel _client_end) {
  return ResultOf::GetReportsEvent(std::move(_client_end));
}

template <>
InputDevice::UnownedResultOf::GetReportsEvent_Impl<InputDevice::GetReportsEventResponse>::GetReportsEvent_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  FIDL_ALIGNDECL uint8_t _write_bytes[sizeof(GetReportsEventRequest)] = {};
  ::fidl::BytePart _request_buffer(_write_bytes, sizeof(_write_bytes));
  memset(_request_buffer.data(), 0, GetReportsEventRequest::PrimarySize);
  _request_buffer.set_actual(sizeof(GetReportsEventRequest));
  ::fidl::DecodedMessage<GetReportsEventRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      InputDevice::InPlace::GetReportsEvent(std::move(_client_end), std::move(_response_buffer)));
}

InputDevice::UnownedResultOf::GetReportsEvent InputDevice::SyncClient::GetReportsEvent(::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetReportsEvent(zx::unowned_channel(this->channel_), std::move(_response_buffer));
}

InputDevice::UnownedResultOf::GetReportsEvent InputDevice::Call::GetReportsEvent(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetReportsEvent(std::move(_client_end), std::move(_response_buffer));
}

::fidl::DecodeResult<InputDevice::GetReportsEventResponse> InputDevice::InPlace::GetReportsEvent(zx::unowned_channel _client_end, ::fidl::BytePart response_buffer) {
  constexpr uint32_t _write_num_bytes = sizeof(GetReportsEventRequest);
  ::fidl::internal::AlignedBuffer<_write_num_bytes> _write_bytes;
  ::fidl::BytePart _request_buffer = _write_bytes.view();
  _request_buffer.set_actual(_write_num_bytes);
  ::fidl::DecodedMessage<GetReportsEventRequest> params(std::move(_request_buffer));
  InputDevice::SetTransactionHeaderFor::GetReportsEventRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetReportsEventResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<GetReportsEventRequest, GetReportsEventResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetReportsEventResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
InputDevice::ResultOf::GetReports_Impl<InputDevice::GetReportsResponse>::GetReports_Impl(zx::unowned_channel _client_end) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetReportsRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, GetReportsRequest::PrimarySize);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetReportsRequest));
  ::fidl::DecodedMessage<GetReportsRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      InputDevice::InPlace::GetReports(std::move(_client_end), Super::response_buffer()));
}

InputDevice::ResultOf::GetReports InputDevice::SyncClient::GetReports() {
  return ResultOf::GetReports(zx::unowned_channel(this->channel_));
}

InputDevice::ResultOf::GetReports InputDevice::Call::GetReports(zx::unowned_channel _client_end) {
  return ResultOf::GetReports(std::move(_client_end));
}

template <>
InputDevice::UnownedResultOf::GetReports_Impl<InputDevice::GetReportsResponse>::GetReports_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  FIDL_ALIGNDECL uint8_t _write_bytes[sizeof(GetReportsRequest)] = {};
  ::fidl::BytePart _request_buffer(_write_bytes, sizeof(_write_bytes));
  memset(_request_buffer.data(), 0, GetReportsRequest::PrimarySize);
  _request_buffer.set_actual(sizeof(GetReportsRequest));
  ::fidl::DecodedMessage<GetReportsRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      InputDevice::InPlace::GetReports(std::move(_client_end), std::move(_response_buffer)));
}

InputDevice::UnownedResultOf::GetReports InputDevice::SyncClient::GetReports(::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetReports(zx::unowned_channel(this->channel_), std::move(_response_buffer));
}

InputDevice::UnownedResultOf::GetReports InputDevice::Call::GetReports(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetReports(std::move(_client_end), std::move(_response_buffer));
}

::fidl::DecodeResult<InputDevice::GetReportsResponse> InputDevice::InPlace::GetReports(zx::unowned_channel _client_end, ::fidl::BytePart response_buffer) {
  constexpr uint32_t _write_num_bytes = sizeof(GetReportsRequest);
  ::fidl::internal::AlignedBuffer<_write_num_bytes> _write_bytes;
  ::fidl::BytePart _request_buffer = _write_bytes.view();
  _request_buffer.set_actual(_write_num_bytes);
  ::fidl::DecodedMessage<GetReportsRequest> params(std::move(_request_buffer));
  InputDevice::SetTransactionHeaderFor::GetReportsRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetReportsResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<GetReportsRequest, GetReportsResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetReportsResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
InputDevice::ResultOf::GetDescriptor_Impl<InputDevice::GetDescriptorResponse>::GetDescriptor_Impl(zx::unowned_channel _client_end) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetDescriptorRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, GetDescriptorRequest::PrimarySize);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetDescriptorRequest));
  ::fidl::DecodedMessage<GetDescriptorRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      InputDevice::InPlace::GetDescriptor(std::move(_client_end), Super::response_buffer()));
}

InputDevice::ResultOf::GetDescriptor InputDevice::SyncClient::GetDescriptor() {
  return ResultOf::GetDescriptor(zx::unowned_channel(this->channel_));
}

InputDevice::ResultOf::GetDescriptor InputDevice::Call::GetDescriptor(zx::unowned_channel _client_end) {
  return ResultOf::GetDescriptor(std::move(_client_end));
}

template <>
InputDevice::UnownedResultOf::GetDescriptor_Impl<InputDevice::GetDescriptorResponse>::GetDescriptor_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  FIDL_ALIGNDECL uint8_t _write_bytes[sizeof(GetDescriptorRequest)] = {};
  ::fidl::BytePart _request_buffer(_write_bytes, sizeof(_write_bytes));
  memset(_request_buffer.data(), 0, GetDescriptorRequest::PrimarySize);
  _request_buffer.set_actual(sizeof(GetDescriptorRequest));
  ::fidl::DecodedMessage<GetDescriptorRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      InputDevice::InPlace::GetDescriptor(std::move(_client_end), std::move(_response_buffer)));
}

InputDevice::UnownedResultOf::GetDescriptor InputDevice::SyncClient::GetDescriptor(::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetDescriptor(zx::unowned_channel(this->channel_), std::move(_response_buffer));
}

InputDevice::UnownedResultOf::GetDescriptor InputDevice::Call::GetDescriptor(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::GetDescriptor(std::move(_client_end), std::move(_response_buffer));
}

::fidl::DecodeResult<InputDevice::GetDescriptorResponse> InputDevice::InPlace::GetDescriptor(zx::unowned_channel _client_end, ::fidl::BytePart response_buffer) {
  constexpr uint32_t _write_num_bytes = sizeof(GetDescriptorRequest);
  ::fidl::internal::AlignedBuffer<_write_num_bytes> _write_bytes;
  ::fidl::BytePart _request_buffer = _write_bytes.view();
  _request_buffer.set_actual(_write_num_bytes);
  ::fidl::DecodedMessage<GetDescriptorRequest> params(std::move(_request_buffer));
  InputDevice::SetTransactionHeaderFor::GetDescriptorRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetDescriptorResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<GetDescriptorRequest, GetDescriptorResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<InputDevice::GetDescriptorResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}


bool InputDevice::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  switch (hdr->ordinal) {
    case kInputDevice_GetReportsEvent_Ordinal:
    case kInputDevice_GetReportsEvent_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<GetReportsEventRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      impl->GetReportsEvent(
        Interface::GetReportsEventCompleter::Sync(txn));
      return true;
    }
    case kInputDevice_GetReports_Ordinal:
    case kInputDevice_GetReports_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<GetReportsRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      impl->GetReports(
        Interface::GetReportsCompleter::Sync(txn));
      return true;
    }
    case kInputDevice_GetDescriptor_Ordinal:
    case kInputDevice_GetDescriptor_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<GetDescriptorRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      impl->GetDescriptor(
        Interface::GetDescriptorCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool InputDevice::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void InputDevice::Interface::GetReportsEventCompleterBase::Reply(int32_t status, ::zx::event event) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetReportsEventResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<GetReportsEventResponse*>(_write_bytes);
  InputDevice::SetTransactionHeaderFor::GetReportsEventResponse(
      ::fidl::DecodedMessage<GetReportsEventResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetReportsEventResponse::PrimarySize,
              GetReportsEventResponse::PrimarySize)));
  _response.status = std::move(status);
  _response.event = std::move(event);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(GetReportsEventResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<GetReportsEventResponse>(std::move(_response_bytes)));
}

void InputDevice::Interface::GetReportsEventCompleterBase::Reply(::fidl::BytePart _buffer, int32_t status, ::zx::event event) {
  if (_buffer.capacity() < GetReportsEventResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<GetReportsEventResponse*>(_buffer.data());
  InputDevice::SetTransactionHeaderFor::GetReportsEventResponse(
      ::fidl::DecodedMessage<GetReportsEventResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetReportsEventResponse::PrimarySize,
              GetReportsEventResponse::PrimarySize)));
  _response.status = std::move(status);
  _response.event = std::move(event);
  _buffer.set_actual(sizeof(GetReportsEventResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<GetReportsEventResponse>(std::move(_buffer)));
}

void InputDevice::Interface::GetReportsEventCompleterBase::Reply(::fidl::DecodedMessage<GetReportsEventResponse> params) {
  InputDevice::SetTransactionHeaderFor::GetReportsEventResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void InputDevice::Interface::GetReportsCompleterBase::Reply(::fidl::VectorView<::llcpp::fuchsia::input::report::InputReport> reports) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetReportsResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  GetReportsResponse _response = {};
  InputDevice::SetTransactionHeaderFor::GetReportsResponse(
      ::fidl::DecodedMessage<GetReportsResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetReportsResponse::PrimarySize,
              GetReportsResponse::PrimarySize)));
  _response.reports = std::move(reports);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void InputDevice::Interface::GetReportsCompleterBase::Reply(::fidl::BytePart _buffer, ::fidl::VectorView<::llcpp::fuchsia::input::report::InputReport> reports) {
  if (_buffer.capacity() < GetReportsResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  GetReportsResponse _response = {};
  InputDevice::SetTransactionHeaderFor::GetReportsResponse(
      ::fidl::DecodedMessage<GetReportsResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetReportsResponse::PrimarySize,
              GetReportsResponse::PrimarySize)));
  _response.reports = std::move(reports);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void InputDevice::Interface::GetReportsCompleterBase::Reply(::fidl::DecodedMessage<GetReportsResponse> params) {
  InputDevice::SetTransactionHeaderFor::GetReportsResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void InputDevice::Interface::GetDescriptorCompleterBase::Reply(::llcpp::fuchsia::input::report::DeviceDescriptor descriptor) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<GetDescriptorResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  GetDescriptorResponse _response = {};
  InputDevice::SetTransactionHeaderFor::GetDescriptorResponse(
      ::fidl::DecodedMessage<GetDescriptorResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetDescriptorResponse::PrimarySize,
              GetDescriptorResponse::PrimarySize)));
  _response.descriptor = std::move(descriptor);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void InputDevice::Interface::GetDescriptorCompleterBase::Reply(::fidl::BytePart _buffer, ::llcpp::fuchsia::input::report::DeviceDescriptor descriptor) {
  if (_buffer.capacity() < GetDescriptorResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  GetDescriptorResponse _response = {};
  InputDevice::SetTransactionHeaderFor::GetDescriptorResponse(
      ::fidl::DecodedMessage<GetDescriptorResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              GetDescriptorResponse::PrimarySize,
              GetDescriptorResponse::PrimarySize)));
  _response.descriptor = std::move(descriptor);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void InputDevice::Interface::GetDescriptorCompleterBase::Reply(::fidl::DecodedMessage<GetDescriptorResponse> params) {
  InputDevice::SetTransactionHeaderFor::GetDescriptorResponse(params);
  CompleterBase::SendReply(std::move(params));
}



void InputDevice::SetTransactionHeaderFor::GetReportsEventRequest(const ::fidl::DecodedMessage<InputDevice::GetReportsEventRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetReportsEvent_Ordinal);
}
void InputDevice::SetTransactionHeaderFor::GetReportsEventResponse(const ::fidl::DecodedMessage<InputDevice::GetReportsEventResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetReportsEvent_Ordinal);
}

void InputDevice::SetTransactionHeaderFor::GetReportsRequest(const ::fidl::DecodedMessage<InputDevice::GetReportsRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetReports_Ordinal);
}
void InputDevice::SetTransactionHeaderFor::GetReportsResponse(const ::fidl::DecodedMessage<InputDevice::GetReportsResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetReports_Ordinal);
}

void InputDevice::SetTransactionHeaderFor::GetDescriptorRequest(const ::fidl::DecodedMessage<InputDevice::GetDescriptorRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetDescriptor_Ordinal);
}
void InputDevice::SetTransactionHeaderFor::GetDescriptorResponse(const ::fidl::DecodedMessage<InputDevice::GetDescriptorResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kInputDevice_GetDescriptor_Ordinal);
}

}  // namespace report
}  // namespace input
}  // namespace fuchsia
}  // namespace llcpp
