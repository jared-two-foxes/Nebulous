
#include "Resource.h"

using namespace Nebulae;


int32 g_nextIdentifier = 0;

Resource::Resource( const std::string& name, RenderSystem* renderDevice  )
  : m_identifier(g_nextIdentifier++),
    m_name(name), 
    m_loadingStatus(LoadingStatus::Unloaded),
    m_renderDevice(renderDevice)
{
}



Resource::~Resource()
{
}


int32
Resource::GetIdentifier() const
{ return m_identifier; }


const std::string& 
Resource::GetName() const 
{ return m_name; }


bool 
Resource::IsLoaded() const 
{ return (m_loadingStatus == LoadingStatus::Loaded); }


bool 
Resource::Load( File* is )
{
  // If load has already been called successfully then return that success.
  if( m_loadingStatus == LoadingStatus::Loaded ) {
    return true;
  }

  // Flag the resource as attempting to load
  m_loadingStatus = LoadingStatus::Loading;

  if( !LoadImpl_(is) ) {
    m_loadingStatus = LoadingStatus::Unloaded;
    return false;
  }

  // Flag the asset as loaded
  m_loadingStatus = LoadingStatus::Loaded;		

  return true;
}


bool
Resource::Unload()
{ 
  // If load has already been called successfully then return that success.
  if( m_loadingStatus == LoadingStatus::Unloaded ) {
    return true;
  }

  // Flag the resource as attempting to load
  m_loadingStatus = LoadingStatus::Unloading;

  if( !UnloadImpl_() ) {
    m_loadingStatus = LoadingStatus::Loaded;
    return false;
  }

  // Flag the asset as loaded
  m_loadingStatus = LoadingStatus::Unloaded;    

  return true;
}