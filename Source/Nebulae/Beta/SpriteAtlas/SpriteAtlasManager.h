#ifndef __SPRITEATLASMANAGER_H__
#define __SPRITEATLASMANAGER_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae {

//Forward Decleration.
class FileSystem;
class RenderSystem;
class SpriteAtlas;
  

/** SpriteAtlasManager.
	*/
class SpriteAtlasManager
{
public:
  typedef std::shared_ptr<FileSystem >   FileArchivePtr;
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
  
private:
  FileArchivePtr             m_fileSystem;
  RenderSystemPtr            m_renderSystem;
  std::vector<SpriteAtlas* > m_Resources;

	public:
    SpriteAtlasManager( FileArchivePtr fileSystem, RenderSystemPtr pRenderSystem );
		virtual ~SpriteAtlasManager();

		SpriteAtlas* Create( const std::string& name );
		SpriteAtlas* GetByName( const std::string& name ) const;

  protected:
    virtual void addImpl( SpriteAtlas* res );

	}; //SpriteAtlasManager

} //Nebulae

#endif // __SPRITEATLASMANAGER_H__