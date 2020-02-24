// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "esp/core/esp.h"
#include "esp/gfx/magnum.h"
#include <Corrade/Containers/Containers.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/Math/Range.h>

// This class provides routines to:
// set and get local rigid body transformation of the current node w.r.t. the
// parent node; get global rigid body transformation

namespace esp {
namespace scene {

class SceneGraph;

// Future types may include e.g., "LIGHT"
enum class SceneNodeType {
  EMPTY = 0,
  SENSOR = 1,
  AGENT = 2,
  CAMERA = 3,
};

class SceneNode : public MagnumObject {
public:
  // creating a scene node "in the air" is not allowed.
  // it must set an existing node as its parent node.
  // this is to prevent any sub-tree that is "floating in the air", without a
  // terminate node (e.g., "MagnumScene" defined in SceneGraph) as its ancestor
  SceneNode() = delete;
  SceneNode(SceneNode &parent);

  // get the type of the attached object
  SceneNodeType getType() { return type_; }
  void setType(SceneNodeType type) { type_ = type; }

  // Add a feature. Used to avoid naked `new` and makes intent clearer.
  template <class U, class... Args> void addFeature(Args &&... args) {
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    new U{*this, std::forward<Args>(args)...};
  }

  //! Create a new child SceneNode and return it. NOTE: this SceneNode owns and
  //! is responsible for deallocating created child
  //! NOTE: child node inherits parent id by default
  SceneNode &createChild();

  //! Returns node id
  virtual int getId() const { return id_; }

  //! Sets node id
  virtual void setId(int id) { id_ = id; }

  Magnum::Vector3 absoluteTranslation() const {
    return this->absoluteTransformation().translation();
  }

  //! recursively compute the cumulative bounding box of the full scene graph
  //! tree for which this node is the root
  const Magnum::Range3D &computeCumulativeBB();

  //! return the local bounding box for meshes stored at this node
  const Magnum::Range3D &getMeshBB() const { return meshBB_; };

  //! return the global bounding box for the mesh stored at this node
  Corrade::Containers::Optional<Magnum::Range3D> getAbsoluteAABB() {
    return aabb_;
  };

  //! return the cumulative bounding box of the full scene graph tree for which
  //! this node is the root
  const Magnum::Range3D &getCumulativeBB() const { return cumulativeBB_; };

  //! set local bounding box for meshes stored at this node
  void setMeshBB(Magnum::Range3D meshBB) { meshBB_ = meshBB; };

  //! set the global bounding box for mesh stored in this node
  void setAbsoluteAABB(Magnum::Range3D aabb) { aabb_ = aabb; };

protected:
  // DO not make the following constructor public!
  // it can ONLY be called from SceneGraph class to initialize the scene graph
  friend class SceneGraph;
  SceneNode(MagnumScene &parentNode);

  // the type of the attached object (e.g., sensor, agent etc.)
  SceneNodeType type_ = SceneNodeType::EMPTY;
  int id_ = ID_UNDEFINED;

  //! the local bounding box for meshes stored at this node
  Magnum::Range3D meshBB_;

  //! the cumulative bounding box of the full scene graph tree for which this
  //! node is the root
  Magnum::Range3D cumulativeBB_;

  //! the global bounding box for *static* meshes stored at this node
  //  NOTE: this is different from the local bounding box meshBB_ defined above:
  //  -) it only applies to *static* meshes, NOT dynamic meshes in the scene (so
  //  it is an optional object);
  //  -) it was computed using mesh vertex positions in world space;
  Corrade::Containers::Optional<Magnum::Range3D> aabb_ =
      Corrade::Containers::NullOpt;
};

} // namespace scene
} // namespace esp
