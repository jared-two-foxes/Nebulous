#ifndef __SPRITEANIMATIONMANAGER_H__
#define __SPRITEANIMATIONMANAGER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

//Forward Declerations.
class SpriteAnimation;
class SpriteAtlasManager;

/** 
	*	A basic factory class that is used to create SpriteAnimation objects.  It
	*	manages the life of these objects, acting as an accessor to find a specific
	*	instance based upon an identifier and finally destroys them when required.
	*/
class SpriteAnimationManager
{
public:
  typedef std::shared_ptr<FileSystem >         FileArchivePtr;
  typedef std::shared_ptr<SpriteAtlasManager > AtlasManagerPtr;
  
private:
  FileArchivePtr                 m_fileSystem;
  AtlasManagerPtr                m_atlasManager;
	std::vector<SpriteAnimation* > m_resources;

	public:
		/** \name Structors */ ///@{
    SpriteAnimationManager( const FileArchivePtr& fileSystem, const AtlasManagerPtr& atlasManager );
		virtual ~SpriteAnimationManager();
    //@}

		/** \name Mutators */ ///@{
		SpriteAnimation* Create( const std::string& name );
		SpriteAnimation* GetByName( const std::string& name ) const;
    //@}

	private:
		/** \name Mutators */ ///@{
		virtual void addImpl( SpriteAnimation* res );
		//@}

}; //SpriteAnimationManager

} //Nebulae

#endif // __SPRITEANIMATIONMANAGER_H__