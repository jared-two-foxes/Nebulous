#ifndef NEBULAE_ALPHA_SAMPLER_SAMPLER_H_
#define NEBULAE_ALPHA_SAMPLER_SAMPLER_H_

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
    virtual bool Load() { return false; }
    virtual bool Unload() { return false; }
  };

private:
  Impl* m_impl;

public:
  Sampler( const std::string& name, RenderSystem* renderer );
  virtual ~Sampler();

  void SetImpl( Impl* impl );
  Impl* GetImpl() const;

private:
  virtual bool LoadImpl_( File* is ) override;
  virtual bool UnloadImpl_() override;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_SAMPLER_SAMPLER_H_