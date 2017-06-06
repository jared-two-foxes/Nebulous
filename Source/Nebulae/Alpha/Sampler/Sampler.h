#ifndef NEBULAE_ALPHA_SAMPLER_H__
#define NEBULAE_ALPHA_SAMPLER_H__

#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae
{

/** Sampler.
 */
class Sampler : public Resource
{
public:
  class Impl
  {
  public:
    Impl() {}
    virtual ~Impl() {}
    virtual bool Load()   { return false; }
    virtual bool Unload() { return false; }
  }; 

private:
  Impl* m_impl;

  public:
    Sampler( const std::string& name, RenderSystem* renderer );
    virtual ~Sampler();

    void  SetImpl( Impl* impl );
    Impl* GetImpl() const;
    
  private:
    virtual bool LoadImpl_( File* is ) override;
    virtual bool UnloadImpl_() override;

};

}

#endif // NEBULAE_ALPHA_SAMPLER_H__