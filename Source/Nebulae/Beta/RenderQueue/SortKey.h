#ifndef NEBULAE_BETA_RENDERQUEUE_SORTKEY_H__
#define NEBULAE_BETA_RENDERQUEUE_SORTKEY_H__

#include <Nebulae/Beta/Material/Pass.h>

namespace Nebulae
{

/// Pack a layer index and transparency flag into a sort key.
/// Opaque (non-transparent) itesm sort before transparent ones within the
/// same layer
inline int MakeSortKey( const Pass* pass, int layer )
{
  // Simple packing: layer in high bits, transparency in low bit.
  // FOr now, transparency detection is minimal - defaults to opaque (0).
  bool isTransparent = pass->GetBlendState().isTransparent;
  return ( layer << 1 ) | ( isTransparent ? 1 : 0 );
}

} // namespace Nebulae

#endif // NEBULAE_BETA_RENDERQUEUE_SORTKEY_H__
