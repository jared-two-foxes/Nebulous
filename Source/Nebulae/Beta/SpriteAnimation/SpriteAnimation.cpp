
#include "SpriteAnimation.h"

#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>

using namespace Nebulae;

	
SpriteAnimation::SpriteAnimation( const std::string& strName, const AtlasManagerPtr& atlasManager )
: m_name(strName),
  m_atlasManager(atlasManager)
{}

	
SpriteAnimation::~SpriteAnimation()
{
	for( std::size_t i = 0, n = m_Frames.size(); i<n; ++i ) {
		delete m_Frames[i];
	}
	m_Frames.clear();
}

	
const std::string& 
SpriteAnimation::GetName() const 
{ return m_name; }


std::size_t 
SpriteAnimation::GetFrameCount() const
{ return m_Frames.size(); }


SpriteAnimationFrame* 
SpriteAnimation::GetFrame( std::size_t idx ) const 
{ return m_Frames[idx]; }


bool
SpriteAnimation::Load( File* stream )
{
  NE_ASSERT( stream, "Null stream passed to sprite animation." )();

//
// Read the contents of stream into a json object.
//
  if( !stream )
  {
    return false;
  }

  Json::Value root;
  bool success = ParseJSON( *stream, &root );
	if( !success) {
		return false;
	}

//
// Grab the frames.
//
	int i = 0;
	SpriteAtlas* pAtlas = NULL;
  Json::Value animation = root["Animation"];
  NE_ASSERT( !animation.isNull() && animation.isObject(), "Found invalid animation node while parsing sprite animation." )();
  Json::Value frames    = animation["Frame"];
  NE_ASSERT( !frames.isNull() && frames.isObject(), "Found invalid Frame node while parsing sprite animation." )();

  for ( int i = 0; i < frames.size(); ++i )
  {
    Json::Value frame = frames[i];
    NE_ASSERT( !frame.isNull() && frame.isObject(), "Found invalid list object while parsing sprite animation." )();
    std::string strSpriteSheetName = json_cast<const char* >( frame["spritesheet"] );
    std::string strFrameName       = json_cast<const char* >( frame["frame"] );

    // Grab or create the Sprite Atlas.
    pAtlas = m_atlasManager->GetByName( strSpriteSheetName );
    if ( pAtlas == NULL ) {
      pAtlas = m_atlasManager->Create( strSpriteSheetName );
    }

    // Create the animation frame link.
    SpriteAnimationFrame* pAnimationFrame = new SpriteAnimationFrame();
    pAnimationFrame->iId = i++;
    pAnimationFrame->pSpriteAtlas = pAtlas;
    pAnimationFrame->strFrameName = strFrameName;

    // Store the reference frame.
    m_Frames.push_back(pAnimationFrame);
  }

	return true;
}
