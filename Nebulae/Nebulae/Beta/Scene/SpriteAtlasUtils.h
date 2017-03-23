#ifndef __NEBULAE_SPRITEATLASUTILS_H__
#define __NEBULAE_SPRITEATLASUTILS_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class  Material;
class  RenderSystem; 
class  SceneNode;
class  SceneObject;
class  SpriteAtlas;
struct SpriteAtlasModule;
class  SubTexture;
class  Texture;


enum SpriteAtlasFlags {
	SAF_NONE  = 0,
	SAF_FLIPX = 1,
	SAF_FLIPY = 2
};

struct SpriteAtlasUtils
{

  /** 
   *  Attaches a Sprite Frame to a scene node.  This will create a new SceneObject and attach the frame
   *  to it.
   *  
   *  @return
   *    Nothing.
   */
  static SceneObject* AttachFrameToNode( std::weak_ptr<RenderSystem > renderer, Material* material, SceneNode* pNode,
    SpriteAtlas* pSpriteAtlas, const std::string& strFrameName, int iFlags = 0 );


  /** 
   *  Attaches a Sprite Frame to an existing SceneObject.  Note that it does not assume that the SceneObject
   *  has a SubObject with this material already associated with it, if it does not find a SubObject it will
   *  create one.
   *  
   *  @return
   *    Nothing.
   */
  static void SetSpriteFrame( std::weak_ptr<RenderSystem > renderer, Material* material, SceneObject* pSubObject,
    SpriteAtlas* pAtlas, const std::string& strFrameName, int iFlags = 0 );


  /**
   *  Accepts 2 SubTextures which are assumed to the of the same dimension and occupy the same space (ie overlap)
   *  and checks each pixel for any point in which both textures have an alpha value greater than @param alphaValue.
   * 
   *  @param texture1
   *    the first texture
   *  
   *  @param texture2
   *    the second texture
   *    
   *  @return
   *    true  - visible pixel was found for both textures at the same pixel location.
   *    false - no visible pixel collision was determined.
   */
  static bool CheckForPixelCollision( SubTexture& texture1, int flags1, SubTexture& texture2, int flags2, uint8 alphaValue = 128 );

};

}

#endif // __NEBULAE_SPRITEATLASUTILS_H__