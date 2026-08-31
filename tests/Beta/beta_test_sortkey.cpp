#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/RenderQueue/SortKey.h>

#include "gtest/gtest.h"

using namespace Nebulae;

TEST( SortKey, MakeSortKeyIncludesPassTransparency )
{
  Pass opaquePass;
  Pass transparentPass;
  transparentPass.SetBlendState( BlendState{ true } );

  const int opaqueKey = MakeSortKey( &opaquePass, 4 );
  const int transparentKey = MakeSortKey( &transparentPass, 4 );

  EXPECT_EQ( 8, opaqueKey );
  EXPECT_EQ( 9, transparentKey );
  EXPECT_NE( opaqueKey, transparentKey );
}
