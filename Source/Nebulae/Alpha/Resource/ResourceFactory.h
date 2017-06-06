#ifndef NEBULAE_COMMON_RESOURCEFACTORY_H__
#define NEBULAE_COMMON_RESOURCEFACTORY_H__

#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae {

class RenderSystem;

template <class T>
class ResourceFactory
{
private:
  RenderSystem&    m_renderSystem; 
  std::vector<T* > m_resources;

  public:
    ResourceFactory( RenderSystem& renderSystem );
    virtual ~ResourceFactory();

    const std::vector<T* >& GetResources() const;
    T* FindByIdentifier( int32 identifier ) const;
    T* FindByName( const char* name ) const;

    T* Create( const char* name );

  private:
    bool AddToLists( T* resource );

};


template <class T>
ResourceFactory<T>::ResourceFactory( RenderSystem& renderSystem ) 
: m_renderSystem( renderSystem )
{}

template <class T>
ResourceFactory<T>::~ResourceFactory() {
  for( std::size_t i = 0, n = m_resources.size(); i < n; ++i ) {
    delete m_resources[i];
  }
  m_resources.clear();
}


template <class T> 
const std::vector<T* >& 
ResourceFactory<T>::GetResources() const {
  return m_resources;
}

template <class T> 
T* 
ResourceFactory<T>::FindByIdentifier( int32 identifier ) const {
  std::vector<T* >::const_iterator end_it = m_resources.end();
  for( std::vector<T* >::const_iterator it = m_resources.begin(); it != end_it; ++it ) {
    if( (*it)->GetIdentifier() == identifier ) {
      return static_cast<T*>(*it);
    }
  }

  return nullptr;
}

template <class T> 
T* 
ResourceFactory<T>::FindByName( const char* name ) const {
  std::vector<T* >::const_iterator end_it = m_resources.end();
  for( std::vector<T* >::const_iterator it = m_resources.begin(); it != end_it; ++it ) {
    if( (*it)->GetName().compare( name ) == 0 ) {
      return static_cast<T*>(*it);
    }
  }

  return NULL;
}

template <class T> 
T*
ResourceFactory<T>::Create( const char* name )
{
  // If the resource was requested without a name then give an auto generated name.
  std::string adjustedName = name == nullptr ? "" : name;
  if( trim(adjustedName) == "" ) {
    char       temp[64];
    static int randomResourceNumberCounter = 0;
    sprintf( temp, "res_guid_%d", randomResourceNumberCounter++ ); 
    adjustedName = temp;
  }

  // Attempt to find a resource with the matching name, returning it if found.
  T* resource = FindByName( adjustedName.c_str() );
  if( resource ) {
    NE_ASSERT( resource != NULL, "Resource with name already exists." )( adjustedName );
    return resource;
  }

  // Create resource & add it to the internal lists.
  resource = new T( adjustedName, &m_renderSystem );
  if( resource && AddToLists(resource) ) {
    return resource;
  }

  // Failed to add the resource, delete it and return NULL.
  delete resource;
  return NULL;
}


template <class T> 
bool 
ResourceFactory<T>::AddToLists( T* resource ) {
  m_resources.push_back( resource ); //should never fail.
  return true;
}

}

#endif // NEBULAE_COMMON_RESOURCEFACTORY_H__