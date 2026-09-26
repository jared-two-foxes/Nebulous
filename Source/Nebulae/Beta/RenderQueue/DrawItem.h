#ifndef NEBULAE_BETA_RENDERQUEUE_DRAWITEM_H_
#define NEBULAE_BETA_RENDERQUEUE_DRAWITEM_H_

#include <cstddef>

namespace Nebulae
{
class SceneObject;
class SceneNode;
class Pass;

struct DrawItem
{
  int sortKey = 0;
  int submissionOrder = 0;
  SceneObject* object = nullptr;
  const SceneNode* node = nullptr;
  const Pass* pass = nullptr;
  std::size_t slotIndex = 0;
};

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_DRAWITEM_H_
