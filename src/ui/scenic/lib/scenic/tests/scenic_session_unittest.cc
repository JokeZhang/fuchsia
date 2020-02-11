// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/gtest/test_loop_fixture.h>

#include "gtest/gtest.h"
#include "lib/ui/scenic/cpp/commands.h"
#include "src/ui/scenic/lib/gfx/engine/session.h"
#include "src/ui/scenic/lib/scenic/session.h"
#include "src/ui/scenic/lib/utils/helpers.h"

namespace scenic_impl {
namespace test {

class MockGfxSession : public gfx::Session {
 public:
  MockGfxSession(scheduling::SessionId session_id = 1) : Session(session_id, {}){};

  bool ScheduleUpdateForPresent(zx::time presentation_time, std::vector<zx::event> release_fences,
                                fuchsia::ui::scenic::Session::PresentCallback callback) override {
    ++present_called_count_;
    return true;
  }

  bool ScheduleUpdateForPresent2(zx::time requested_presentation_time,
                                 std::vector<zx::event> release_fences,
                                 scheduling::Present2Info present2_info) override {
    ++present2_called_count_;
    return true;
  };

  void DispatchCommand(fuchsia::ui::scenic::Command command) override {
    ++num_commands_dispatched_;
  };

  int present_called_count_ = 0;
  int present2_called_count_ = 0;
  int num_commands_dispatched_ = 0;
};

class ScenicSessionTest : public ::gtest::TestLoopFixture {
 public:
  ScenicSessionTest() = default;
  ~ScenicSessionTest() override = default;

  void InitializeSession(Session& session) {
    std::array<CommandDispatcherUniquePtr, System::TypeId::kMaxSystems> dispatchers;
    dispatchers[System::TypeId::kGfx] = CommandDispatcherUniquePtr(delegate_.get(), [](auto) {});
    session.SetCommandDispatchers(std::move(dispatchers));
  }

  std::unique_ptr<MockGfxSession> delegate_;

 protected:
  void SetUp() override { delegate_ = std::make_unique<MockGfxSession>(); }

  void TearDown() override { delegate_.reset(); }
};

class TestSessionListener : public fuchsia::ui::scenic::SessionListener {
 public:
  TestSessionListener() = default;

  // |fuchsia::ui::scenic::SessionListener|
  void OnScenicEvent(std::vector<fuchsia::ui::scenic::Event> events) override {
    events_.reserve(events_.size() + events.size());
    std::move(events.begin(), events.end(), std::inserter(events_, events_.end()));
  }
  void OnScenicError(std::string error) override {}

  std::vector<fuchsia::ui::scenic::Event> events_;
};

TEST_F(ScenicSessionTest, EventReporterFiltersViewDetachedAndAttachedEvents) {
  TestSessionListener test_session_listener;
  fidl::Binding<fuchsia::ui::scenic::SessionListener> session_listener(&test_session_listener);
  fidl::InterfaceHandle<fuchsia::ui::scenic::SessionListener> session_listener_handle;
  session_listener.Bind(session_listener_handle.NewRequest());

  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               std::move(session_listener_handle),
                               /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Check single Attach event.
  const uint32_t kViewId1 = 12;
  fuchsia::ui::gfx::Event attached_event_1;
  attached_event_1.set_view_attached_to_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(attached_event_1));
  RunLoopUntilIdle();
  EXPECT_EQ(test_session_listener.events_.size(), 1u);
  EXPECT_EQ(fuchsia::ui::gfx::Event::Tag::kViewAttachedToScene,
            test_session_listener.events_[0].gfx().Which());

  // Check single Attach event.
  fuchsia::ui::gfx::Event attached_event_2;
  attached_event_2.set_view_attached_to_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(attached_event_2));
  fuchsia::ui::gfx::Event detached_event_1;
  detached_event_1.set_view_detached_from_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(detached_event_1));
  RunLoopUntilIdle();
  EXPECT_EQ(test_session_listener.events_.size(), 1u);

  // Check Detach-Attach pair.
  fuchsia::ui::gfx::Event detached_event_2;
  detached_event_2.set_view_detached_from_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(detached_event_2));
  fuchsia::ui::gfx::Event attached_event_3;
  attached_event_3.set_view_attached_to_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(attached_event_3));
  fuchsia::ui::input::InputEvent input_event;
  session.event_reporter()->EnqueueEvent(std::move(input_event));
  RunLoopUntilIdle();
  EXPECT_EQ(test_session_listener.events_.size(), 1u);

  // Check Detach-Attach pair with different view ids.
  fuchsia::ui::gfx::Event detached_event_3;
  detached_event_3.set_view_detached_from_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(detached_event_3));
  const uint32_t kViewId2 = 23;
  fuchsia::ui::gfx::Event attached_event_4;
  attached_event_4.set_view_attached_to_scene({.view_id = kViewId2});
  session.event_reporter()->EnqueueEvent(std::move(attached_event_4));
  RunLoopUntilIdle();
  EXPECT_EQ(test_session_listener.events_.size(), 3u);
  EXPECT_EQ(fuchsia::ui::gfx::Event::Tag::kViewDetachedFromScene,
            test_session_listener.events_[1].gfx().Which());
  EXPECT_EQ(fuchsia::ui::gfx::Event::Tag::kViewAttachedToScene,
            test_session_listener.events_[2].gfx().Which());

  // Check Detach-Attach-Detach sequence.
  fuchsia::ui::gfx::Event detached_event_4;
  detached_event_4.set_view_detached_from_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(detached_event_4));
  fuchsia::ui::gfx::Event attached_event_5;
  attached_event_5.set_view_attached_to_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(attached_event_5));
  fuchsia::ui::gfx::Event detached_event_5;
  detached_event_5.set_view_detached_from_scene({.view_id = kViewId1});
  session.event_reporter()->EnqueueEvent(std::move(detached_event_5));
  RunLoopUntilIdle();
  EXPECT_EQ(test_session_listener.events_.size(), 4u);
  EXPECT_EQ(fuchsia::ui::gfx::Event::Tag::kViewDetachedFromScene,
            test_session_listener.events_[1].gfx().Which());
}

// Tests creating a session, and calling Present with two acquire fences. The call should not be
// propagated further until all fences have been signalled.
TEST_F(ScenicSessionTest, AcquireFences_WithPresent1) {
  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               /*listener=*/nullptr, /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Create acquire fences.
  zx::event acquire_fence1, acquire_fence2;
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence1));
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence2));
  std::vector<zx::event> acquire_fences;
  acquire_fences.push_back(utils::CopyEvent(acquire_fence1));
  acquire_fences.push_back(utils::CopyEvent(acquire_fence2));

  // Call Present with the acquire fences.
  session.Present(0u, std::move(acquire_fences), {}, [](auto) {});
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present_called_count_, 0);

  acquire_fence1.signal(0u, escher::kFenceSignalled);
  // Nothing should have happened.
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present_called_count_, 0);

  acquire_fence2.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present_called_count_, 1);
}

// Tests creating a session, and calling Present with two acquire fences. The call should not be
// propagated further until all fences have been signalled.
TEST_F(ScenicSessionTest, AcquireFences_WithPresent2) {
  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               /*listener=*/nullptr, /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Create acquire fences.
  zx::event acquire_fence1, acquire_fence2;
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence1));
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence2));
  std::vector<zx::event> acquire_fences;
  acquire_fences.push_back(utils::CopyEvent(acquire_fence1));
  acquire_fences.push_back(utils::CopyEvent(acquire_fence2));

  // Call Present with the acquire fences.
  session.Present2(utils::CreatePresent2Args(0, std::move(acquire_fences), {}, 0), [](auto) {});
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present2_called_count_, 0);

  acquire_fence1.signal(0u, escher::kFenceSignalled);
  // Nothing should have happened.
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present2_called_count_, 0);

  acquire_fence2.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present2_called_count_, 1);
}

// Tests creating a session, and calling Present twice with different sets of acquire fences.
TEST_F(ScenicSessionTest, AcquireFences_WithMultiplePresent1) {
  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               /*listener=*/nullptr, /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Create acquire fences.
  zx::event acquire_fence1, acquire_fence2;
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence1));
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence2));

  {
    std::vector<zx::event> acquire_fences1;
    acquire_fences1.push_back(utils::CopyEvent(acquire_fence1));
    session.Present(0u, std::move(acquire_fences1), {}, [](auto) {});
  }
  {
    std::vector<zx::event> acquire_fences2;
    acquire_fences2.push_back(utils::CopyEvent(acquire_fence2));
    session.Present(0u, std::move(acquire_fences2), {}, [](auto) {});
  }
  // Call with no fences.
  session.Present(0u, {}, {}, [](auto) {});

  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present_called_count_, 0);

  acquire_fence1.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // Only the first call should have been made.
  EXPECT_EQ(delegate_->present_called_count_, 1);

  acquire_fence2.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // Both the remaining calls should have been made.
  EXPECT_EQ(delegate_->present_called_count_, 3);
}

// Tests creating a session, and calling Present twice with different sets of acquire fences.
TEST_F(ScenicSessionTest, AcquireFences_WithMultiplePresent2) {
  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               /*listener=*/nullptr, /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Create acquire fences.
  zx::event acquire_fence1, acquire_fence2;
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence1));
  ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence2));

  {
    std::vector<zx::event> acquire_fences1;
    acquire_fences1.push_back(utils::CopyEvent(acquire_fence1));
    session.Present2(utils::CreatePresent2Args(0, std::move(acquire_fences1), {}, 0), [](auto) {});
  }
  {
    std::vector<zx::event> acquire_fences2;
    acquire_fences2.push_back(utils::CopyEvent(acquire_fence2));
    session.Present2(utils::CreatePresent2Args(0, std::move(acquire_fences2), {}, 0), [](auto) {});
  }
  // Call with no fences.
  session.Present2(utils::CreatePresent2Args(0, {}, {}, 0), [](auto) {});

  RunLoopUntilIdle();
  EXPECT_EQ(delegate_->present2_called_count_, 0);

  acquire_fence1.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // Only the first call should have been made.
  EXPECT_EQ(delegate_->present2_called_count_, 1);

  acquire_fence2.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // Both the remaining calls should have been made.
  EXPECT_EQ(delegate_->present2_called_count_, 3);
}

// This tests checks that commands enqueued for separate Present aren't dispatched until (at least)
// the previous Present call has been made.
TEST_F(ScenicSessionTest, CommandForDifferentPresents_MustBeEnqueuedSeparately) {
  fuchsia::ui::scenic::SessionPtr session_ptr;
  scenic_impl::Session session(/*id=*/1, session_ptr.NewRequest(),
                               /*listener=*/nullptr, /*destroy_session_function*/ [] {});
  InitializeSession(session);

  // Enqueue a command.
  fuchsia::ui::scenic::Command cmd1;
  cmd1.set_gfx(::fuchsia::ui::gfx::Command());
  std::vector<fuchsia::ui::scenic::Command> cmds1;
  cmds1.push_back(std::move(cmd1));
  session.Enqueue(std::move(cmds1));

  zx::event acquire_fence1, acquire_fence2;
  {
    ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence1));
    std::vector<zx::event> acquire_fences1;
    acquire_fences1.push_back(utils::CopyEvent(acquire_fence1));
    session.Present2(utils::CreatePresent2Args(0, std::move(acquire_fences1), {}, 0), [](auto) {});
  }

  // Enqueue a command for the second batch.
  fuchsia::ui::scenic::Command cmd2;
  cmd2.set_gfx(::fuchsia::ui::gfx::Command());
  std::vector<fuchsia::ui::scenic::Command> cmds2;
  cmds2.push_back(std::move(cmd2));
  session.Enqueue(std::move(cmds2));

  {
    ASSERT_EQ(ZX_OK, zx::event::create(0, &acquire_fence2));
    std::vector<zx::event> acquire_fences2;
    acquire_fences2.push_back(utils::CopyEvent(acquire_fence2));
    session.Present2(utils::CreatePresent2Args(0, std::move(acquire_fences2), {}, 0), [](auto) {});
  }

  // The first command could have been safely dispatched.
  RunLoopUntilIdle();
  EXPECT_LE(delegate_->num_commands_dispatched_, 1);

  acquire_fence1.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // The first command must have been dispatched before the call Present2 finished, and the second
  // could have been safely dispatched afterwards.
  EXPECT_GE(delegate_->num_commands_dispatched_, 1);
  EXPECT_LE(delegate_->num_commands_dispatched_, 2);

  acquire_fence2.signal(0u, escher::kFenceSignalled);
  RunLoopUntilIdle();
  // After present, the dispatch must have happened.
  EXPECT_EQ(delegate_->num_commands_dispatched_, 2);
}

}  // namespace test
}  // namespace scenic_impl
