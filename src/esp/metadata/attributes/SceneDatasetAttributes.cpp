// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "SceneDatasetAttributes.h"

namespace esp {
namespace metadata {
namespace attributes {

SceneDatasetAttributes::SceneDatasetAttributes(
    const std::string& datasetName,
    const managers::PhysicsAttributesManager::ptr& physAttrMgr)
    : AbstractAttributes("SceneDatasetAttributes", datasetName) {
  assetAttributesManager_ = managers::AssetAttributesManager::create();
  lightLayoutAttributesManager_ =
      managers::LightLayoutAttributesManager::create();
  objectAttributesManager_ = managers::ObjectAttributesManager::create();
  objectAttributesManager_->setAssetAttributesManager(assetAttributesManager_);
  sceneAttributesManager_ = managers::SceneAttributesManager::create();
  stageAttributesManager_ = managers::StageAttributesManager::create(
      objectAttributesManager_, physAttrMgr);
}  // ctor

}  // namespace attributes
}  // namespace metadata
}  // namespace esp
