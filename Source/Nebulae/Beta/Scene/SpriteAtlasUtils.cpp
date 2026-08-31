
#include "SpriteAtlasUtils.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Shaders/UniformBinder.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>


using namespace Nebulae;


const float g_fBillboardVertices[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };


SceneObject* SpriteAtlasUtils::AttachFrameToNode( std::weak_ptr<RenderSystem> renderer, Material* material,
                                                  SceneNode* pNode, SpriteAtlas* pSpriteAtlas,
                                                  const std::string& strFrameName, int iFlags )
{
  if ( renderer.expired() || pNode == nullptr || material == nullptr )
  {
    return nullptr;
  }

  SceneObject* pObj = pNode->CreateObject( material );
  if ( pObj )
  {
    SetSpriteFrame( renderer, material, pObj, pSpriteAtlas, strFrameName, iFlags );
  }

  return pObj;
}


void SpriteAtlasUtils::SetSpriteFrame( std::weak_ptr<RenderSystem> renderer, Material* material, SceneObject* pObj,
                                       SpriteAtlas* pSpriteAtlas, const std::string& strFrameName, int iFlags )
{
  NE_ASSERT( !renderer.expired(), "" );
  NE_ASSERT( material, "" );
  NE_ASSERT( pObj, "" );
  NE_ASSERT( pSpriteAtlas, "" );

  // Early out if there are any issues with the parameters.
  if ( renderer.expired() || material == nullptr || pObj == nullptr || pSpriteAtlas == nullptr )
  {
    return;
  }

  std::shared_ptr<RenderSystem> renderDevicePtr = renderer.lock();
  //
  // Grab the specific frame that we are attempting to add to the scene.
  //
  SubTexture* subTexture = pSpriteAtlas->FindModuleSubTexture( strFrameName );
  NE_ASSERT( subTexture, "" );
  if ( subTexture == nullptr )
  {
    return;
  }

  //
  // Setup the Geometry struct for render operation.
  //
  // Grab the vertex buffer.
  HardwareBuffer* buffer = renderDevicePtr->FindBufferByName( "billboardVertexBuffer" );
  if ( buffer == nullptr )
  {
    // Vertex buffer wasn't found, create it.
    buffer = renderDevicePtr->CreateBuffer( "billboardVertexBuffer", HBU_STATIC_WRITE_ONLY, 12 * sizeof( float ),
                                            HBB_VERTEX, (void*)&g_fBillboardVertices[0] );
  }

  // Create the vertex description.
  VertexDeceleration* pVertexDecl = new VertexDeceleration( 2 );
  pVertexDecl->AddElement( VET_FLOAT2, VES_POSITION, 0 );

  // Create Geometry.
  Geometry* pGeometry = new Geometry();
  pGeometry->m_vertexBuffer = buffer;
  pGeometry->m_vertexDeceleration = pVertexDecl;
  pGeometry->m_vertexCount = 6;
  // pGeometry->m_VertexSize       = 2*sizeof(float);
  pGeometry->m_primitiveTopology = OT_TRIANGLES;

  //
  // Setup uniforms via keyed providers.
  //
  float width = static_cast<float>( subTexture->GetWidth() );
  float height = static_cast<float>( subTexture->GetHeight() );
  float minU = iFlags & SAF_FLIPX ? subTexture->GetTexCoords()[2] : subTexture->GetTexCoords()[0];
  float minV = iFlags & SAF_FLIPY ? subTexture->GetTexCoords()[3] : subTexture->GetTexCoords()[1];
  float maxU = iFlags & SAF_FLIPX ? subTexture->GetTexCoords()[0] : subTexture->GetTexCoords()[2];
  float maxV = iFlags & SAF_FLIPY ? subTexture->GetTexCoords()[1] : subTexture->GetTexCoords()[3];
  const Texture* texturePtr = subTexture->GetTexture();

  pObj->AddProvider( "sprite",
                     [=]( UniformBinder& binder )
                     {
                       binder.Set( "size", Vector2( width, height ) );
                       binder.Set( "offset", Vector2( 0.0f, 0.0f ) );
                       binder.Set( "min_uv", Vector2( minU, minV ) );
                       binder.Set( "max_uv", Vector2( maxU, maxV ) );
                       binder.SetTexture( "diffuseTexture", texturePtr, 0 );
                     } );

  //
  // Iterate and setup passes.
  //
  if ( material->GetPassCount() > 0 )
  {
    // Attempt to grab the input layout for this pass.
    InputLayout* inputLayout =
      renderDevicePtr->FindInputLayoutByUsage( pVertexDecl, material->GetPass( 0 )->GetVertexShader() );
    if ( inputLayout == nullptr )
    {
      inputLayout = renderDevicePtr->CreateInputLayout( "", pVertexDecl, material->GetPass( 0 )->GetVertexShader() );
    }

    // Set pass data for object.
    pObj->SetSlotGeometry( 0, pGeometry );
    pObj->SetSlotInputLayout( 0, inputLayout );
  }
}


bool SpriteAtlasUtils::CheckForPixelCollision( SubTexture& subtexture1, int flags1, SubTexture& subtexture2, int flags2,
                                               uint8 alphaValue )
{
  Image image1, image2;
  subtexture1.GetTexture()->ConvertToImage( image1 );
  subtexture2.GetTexture()->ConvertToImage( image2 );

  int32 width = std::min<int32>( subtexture1.GetWidth(), subtexture2.GetWidth() );
  int32 height = std::min<int32>( subtexture1.GetHeight(), subtexture2.GetHeight() );
  std::size_t image1Width = image1.getWidth();
  std::size_t image1Height = image1.getHeight();
  std::size_t image2Width = image2.getWidth();
  std::size_t image2Height = image2.getHeight();
  Vector2 min1 = subtexture1.GetMinCoord();
  Vector2 min2 = subtexture2.GetMinCoord();

  if ( flags1 & SAF_FLIPX )
  {
    image1.flipAroundY();
  }
  if ( flags1 & SAF_FLIPY )
  {
    image1.flipAroundX();
  }

  if ( flags2 & SAF_FLIPX )
  {
    image2.flipAroundY();
  }
  if ( flags2 & SAF_FLIPY )
  {
    image2.flipAroundX();
  }

  min1 *= Vector2( static_cast<float>( image1Width ),
                   static_cast<float>( image1Height ) ); // size_t-to-float, image dimensions fit in float
  min2 *= Vector2( static_cast<float>( image2Width ),
                   static_cast<float>( image2Height ) ); // size_t-to-float, image dimensions fit in float

  for ( int32 x = 0; x <= width; x++ )
  {
    for ( int32 y = 0; y <= height; y++ )
    {
      Vector2 obj1adjusted = min1 + Vector2( static_cast<float>( x ),
                                             static_cast<float>( y ) ); // int-to-float, pixel coordinates fit in float
      Vector2 obj2adjusted = min2 + Vector2( static_cast<float>( x ),
                                             static_cast<float>( y ) ); // int-to-float, pixel coordinates fit in float

      Colour colour1 = image1.getColourAt(
        static_cast<std::size_t>( obj1adjusted.x ),
        static_cast<std::size_t>( obj1adjusted.y ) ); // float-to-size_t, pixel coordinates are non-negative
      Colour colour2 = image2.getColourAt(
        static_cast<std::size_t>( obj2adjusted.x ),
        static_cast<std::size_t>( obj2adjusted.y ) ); // float-to-size_t, pixel coordinates are non-negative

      if ( colour1.a >= alphaValue && colour2.a >= alphaValue )
      {
        return true;
      }
    }
  }

  return false;
}
