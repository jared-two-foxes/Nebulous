
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
SpriteAnimation::Load( File* is )
{
  if( NULL == is ) return false;

//
// Read the contents of stream into a buffer.
//
  is->SeekToEnd();
  std::size_t size = is->Tell();
  char* buffer = new char[size];
  is->Seek( 0 );
  is->Read( buffer, size );

//
// This is actually returning a structure with parse details which is being cast to a boolean.
//
  pugi::xml_document doc; 
  bool bSuccess = doc.load(buffer);
	if( !bSuccess ) {
    delete [] buffer;
		return false;
	}

//
// Grab the frames.
//
	pugi::xml_node root = doc.document_element();
	pugi::xml_node_iterator itr;
	int i = 0;
	SpriteAtlas* pAtlas = NULL;

	for( itr = root.begin(); itr != root.end(); ++itr ) {
		std::string strSpriteSheetName = (*itr).attribute("spritesheet").as_string();
		pAtlas = m_atlasManager->GetByName( strSpriteSheetName );
		if( pAtlas == NULL ) {
			pAtlas = m_atlasManager->Create( strSpriteSheetName );
		}

		// Create the animation frame link.
		SpriteAnimationFrame* pAnimationFrame = new SpriteAnimationFrame();
		pAnimationFrame->iId          = i++;
		pAnimationFrame->pSpriteAtlas = pAtlas;
		pAnimationFrame->strFrameName = (*itr).attribute("frame").as_string();

		// Store the reference frame.
		m_Frames.push_back( pAnimationFrame );
	}

  delete [] buffer;
	return true;
}
