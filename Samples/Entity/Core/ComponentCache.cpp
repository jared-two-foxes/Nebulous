
#include "ComponentCache.h"

using namespace Sample;


template <class T >
class ComponentPoolImpl : public ComponentPool
{
private:
  ComponentType       m_type;
  std::vector<T >     m_components;
  std::vector<int32 > m_unusedIndices;

  public:
    ComponentPoolImpl( ComponentType type ) : m_type( type )  {}

    ~ComponentPoolImpl() { Clear(); }

    void Clear() 
    {
      m_components.clear();
    }

    void Init( int32 count ) 
    {
      m_components.resize( count ); 
      for( int32 i = 0; i < count; ++i )
      { 
        m_unusedIndices.push_back( i );
      }
    }

    ComponentType GetType() const { return m_type; }

    T* GetComponent( int32 identifier ) const
    {
      for( std::vector<T>::const_iterator it = m_components.begin(); it != m_components.end(); ++it ) 
      {
        if( (*it).GetIdentifier() == identifier ) 
        {
          return (*it);
        }
      }

      return NULL;
    }

    const int32 GetNextAvailableIndex() const
    {
      return m_unusedIndices.empty() ? -1 : m_unusedIndices.back();
    }

};

ComponentCache::ComponentCache()
///
/// Constructor
///
{}


void 
ComponentCache::Clear()
///
/// @todo [jared.watt 27-10-2013] Needs to be documented
///
{
  std::for_each( m_pools.begin(), m_pools.end(), [](ComponentPool* pool) { delete pool; });
  m_pools.clear();
}


void 
ComponentCache::Init( int count )
///
/// Creates all of the components for the various pools.
///
/// @param count
///	  The number of components of each type to create.
///
/// @return
///	  Nothing
///
/// @todo [jared.watt 05-10-2013]
///	  Initiating the components in this manner will result in a certain amount of memory wastage,
///   if this becomes a problem its probably possibly to determine before hand how many components
///   of each type are actually required.
///
{
  //m_AnimationComponent.resize( count );
  //m_AspectComponent.resize( count );
  //m_CollisionComponent.resize( count );
  //m_MotionComponent.resize( count );
  //m_ParticleComponent.resize( count );
  //m_PlacementComponent.resize( count );
  //m_PlacementConstraintComponent.resize( count );
  //m_ScoreComponent.resize( count );
}

void 
ComponentCache::AddPoolForComponentType( ComponentType type )
{
  ComponentPool* pool = NULL; //new ComponentPoolImpl<type>();
}

ComponentPool* 
ComponentCache::GetComponentPool( ComponentType type ) const
{
  for( uint32 i = 0, n = m_pools.size(); i < n; ++i )
  {
    if( m_pools[i]->GetType() == type )
    {
      return m_pools[i];
    }
  }

  return NULL;
}