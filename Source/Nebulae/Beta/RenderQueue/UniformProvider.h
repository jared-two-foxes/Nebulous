#ifndef NEBULAE_BETA_RENDERQUEUE_UNIFORMPROVIDER_H_
#define NEBULAE_BETA_RENDERQUEUE_UNIFORMPROVIDER_H_

#include <Nebulae/Alpha/Shaders/UniformBinder.h>

#include <functional>

namespace Nebulae
{

/// UniformProvider — functional callback that binds uniforms for a draw call.
///
/// Replaces the inheritance-based UniformParameters API. A provider is a
/// std::function<void(UniformBinder&)> — it captures everything it needs
/// (camera, transforms, sprite state) and calls Set/SetTexture on the binder.
///
/// The signature is minimal and domain-free: no Camera parameter. The
/// scene-scope camera provider captures its camera via MakeCameraProvider
/// (Phase 5.1), constructed per view at compile start. This also makes
/// multi-camera/multi-viewport rendering cleaner — each view's compile
/// gets its own captured camera.
///
/// Lifetime rule: captured raw pointers (Camera*, SubTexture*, state structs)
/// must outlive the provider's registration. This is the same constraint as
/// the old UniformParameters API.
///
/// Registration: AddProvider(key, fn) is added to SceneObject in Phase 4.
/// Keyed so re-registration replaces rather than accumulates.
using UniformProvider = std::function<void( UniformBinder& )>;

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_UNIFORMPROVIDER_H_
