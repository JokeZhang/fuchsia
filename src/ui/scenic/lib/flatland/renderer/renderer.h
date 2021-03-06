// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_UI_SCENIC_LIB_FLATLAND_RENDERER_RENDERER_H_
#define SRC_UI_SCENIC_LIB_FLATLAND_RENDERER_RENDERER_H_

#include <fuchsia/sysmem/cpp/fidl.h>
#include <lib/zx/event.h>

#include <optional>

// clang-format off
#include "src/ui/lib/glm_workaround/glm_workaround.h"
// clang-format on

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "src/ui/lib/escher/geometry/types.h"

#include "src/ui/scenic/lib/sysmem/id.h"

namespace flatland {

// Typedef to a flatland type.
using Rectangle2D = escher::Rectangle2D;

// Used to reference a particular flatland image.
using GlobalImageId = uint64_t;

// Contains information regarding the constraints of a particular buffer collection. This
// should be used by the Flatland instance to check that images it wishes to create are
// valid. More information may be added to this struct over time.
struct BufferCollectionMetadata {
  // The number of vmos available in the buffer collection.
  uint32_t vmo_count;

  // Various image constraint information, including max width/height and supported
  // pixel formats. See |constraints.fidl| for more information.
  fuchsia::sysmem::ImageFormatConstraints image_constraints;
};

// Struct representing the data needed to extract an image from a buffer collection.
// All pixel information is stored within the Vmo of the collection so this struct
// only needs information regarding which collection and which vmo to point to, and
// the overall size of the image. Only supports fuchsia::sysmem::PixelFormatType::BGRA32
// as the image format type.
struct ImageMetadata {
  // The unique id of the buffer collection to refer to. This id should match an
  // an ID generated by a call to |Renderer::RegisterBufferCollection| below.
  sysmem_util::GlobalBufferCollectionId collection_id;

  // The unique ID for this particular image.
  GlobalImageId identifier;

  // A single buffer collection may have several vmos. This tells the renderer
  // which vmo in the collection specified by |collection_id| to use as the memory
  // for this image. This value must be less than BufferCollectionMetadata::vmo_count.
  uint32_t vmo_idx;

  // The dimensions of the image in pixels.
  uint32_t width, height;

  bool operator==(const ImageMetadata& meta) const {
    return collection_id == meta.collection_id && vmo_idx == meta.vmo_idx && width == meta.width &&
           height == meta.height;
  }
};

// This is the main renderer interface used by the Flatland System. Since Flatland is
// agnostic to the implementation of the renderer, it is declared here as a virtual
// interface, whose concrete implementation is to be injected into Flatland.
class Renderer {
 public:
  // This function registers a buffer collection with the renderer and is intended to be
  // called by a Flatland instance on that instance's thread, not the render thread. It is
  // thread safe, and so multiple Flatland instances can call it simultaneously. The
  // buffer collection associated with an image must be registered with the renderer before
  // that image can be used during rendering. Concrete implementations of this class may
  // potentially set their own additional constraints on the buffer collection, including
  // negotiating with the display controller.
  //
  // This function returns true upon a successful registration and false otherwise.
  // Registration may fail if the passed in token is invalid or if it is a valid
  // channel but not a valid buffer collection.
  //
  // It is the client's responsibility to provide a GlobalBufferCollectionID and to hold onto
  // that ID to reference the buffer collection in subsequent calls. IDs must be positive,
  // so an ID value of 0 will result in a failed registration.
  //
  // Passing in an ID that is already in use by another collection will also result in a return
  // value of false. An ID can be reused if the buffer collection associated with the ID has
  // been removed from the renderer via a call to |DeregisterBufferCollection|.
  virtual bool RegisterTextureCollection(
      sysmem_util::GlobalBufferCollectionId collection_id,
      fuchsia::sysmem::Allocator_Sync* sysmem_allocator,
      fidl::InterfaceHandle<fuchsia::sysmem::BufferCollectionToken> token) = 0;

  // This function operates exactly like the function |RegisterTextureCollection|, but is
  // specifically for registering collections that contain render targets. In order for an image
  // to be used as a render target in the Render() function below, the buffer collection it
  // is associated with must have been registered with this function.
  //
  // This function is likewise threadsafe, although it is only meant to be called from the render
  // loop, and not by any flatland instance directly.
  virtual bool RegisterRenderTargetCollection(
      sysmem_util::GlobalBufferCollectionId collection_id,
      fuchsia::sysmem::Allocator_Sync* sysmem_allocator,
      fidl::InterfaceHandle<fuchsia::sysmem::BufferCollectionToken> token) = 0;

  // Removes the buffer collection provided from the renderer. All images created using
  // this buffer collection become invalid afterwards and no new images can be created using
  // this collection id. Buffer collections can only be safely deregisted after all pending
  // render work involving the collection has been completed. Deregistering before this point
  // results in undefined behavior and may crash the process. To avoid this, it is possible
  // to synchronize with rendering by passing a release fence in the form of a zx::event to
  // Render() and waiting for it to signal before deregistering. See Render() for more details.
  // This function is threadsafe.
  //
  // Once a buffer collection has been deregistered, the collection_id associated with it will
  // be free to be reused again for a new collection.
  virtual void DeregisterCollection(sysmem_util::GlobalBufferCollectionId collection_id) = 0;

  // This function validates if the buffer collection referenced by |collection_id| is ready to
  // be used in rendering operations and must be called before that buffer collection is used
  // in |Render|. Specifically, this function checks to make sure that the buffer collection is
  // fully allocated (which can only happen after all token holders have set constraints and the
  // client has requested for sysmem to allocate the buffer collection via a call to
  // WaitForBuffersAllocated() or an equivalent function).
  //
  // If the collection has not yet been fully allocated, or if the collection has not been
  // registered with the renderer via a call to RegisterBufferCollection(), this function will
  // return std::nullopt. If the collection has been registered and has been fully allocated, then
  // this function will return a BufferCollectionMetadata struct containing information pertaining
  // to the number of vmos as well as image constraints, such as maximum width/height (see
  // BufferCollectionMetadata at the top of this file for more information).
  //
  // The BufferCollectionMetadata can then be used by the Flatland instance to verify images it
  // wishes to create with this buffer collection. Images map N:1 with buffer collections, since a
  // collection can consist of multiple VMOs, and each VMO in turn can back multiple images. The
  // intended usage then is for the Flatland instance to store the BufferCollectionMetadata it
  // receives and use it to compare against any incoming images. For example, if a buffer collection
  // only has 5 VMOs and an image is referencing vmo index 6, then that would be an error.
  //
  // This function needs to be separate from RegisterBufferCollection() since it is too early to
  // return a |BufferCollectionMetadata| struct at registration time., as the client may have not
  // yet set their constraints, or duplicated their token. Combining this function with the above
  // registration function would require us to block on registration, which could go on forever.
  //
  // Lastly, this function needs to communicate with sysmem and may take some time. However, it does
  // not block indefinitely. Additionally, it only needs to be called once per buffer collection if
  // the call was successful. It is up to the Flatland instance how to proceed in the event of
  // failure. Calling this function multiple times until it returns true is allowed.
  virtual std::optional<BufferCollectionMetadata> Validate(
      sysmem_util::GlobalBufferCollectionId collection_id) = 0;

  // This function is responsible for rendering a single batch of Flatland rectangles into a
  // render target. This function is designed to be called on the render thread, not on any
  // Flatland instance thread. The specific behavior may differ depending on the specific subclass
  // implementation, which may also be a null renderer.
  //
  // This function assumes that clients have already validated their input data by comparing it
  // against the BufferCollectionMetadata received from calling |Validate| above. This function
  // will abort if invalid data has been passed in or if |Validate| has never been called.
  //
  // The size of the arrays |rectangles| and |images| must match or else this function will CHECK.
  // Entries in each array with the same index will be used together in rendering.
  //
  // The vector of release fences will be signaled once rendering has completed. Clients can use
  // these fences to coordinate with other work that needs to wait until rendering is completed
  // to be executed.
  virtual void Render(const ImageMetadata& render_target,
                      const std::vector<Rectangle2D>& rectangles,
                      const std::vector<ImageMetadata>& images,
                      const std::vector<zx::event>& release_fences = {}) = 0;

  virtual ~Renderer() = default;
};

}  // namespace flatland

#endif  // SRC_UI_SCENIC_LIB_FLATLAND_RENDERER_RENDERER_H_
