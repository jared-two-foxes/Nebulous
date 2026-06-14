#ifndef NEBULAE_ALPHA_RESOURCE_H__
#define NEBULAE_ALPHA_RESOURCE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class RenderSystem;

namespace LoadingStatus
{
enum Status
{
  Unloaded,
  Unloading,
  Loading,
  Loaded
};
}

class Resource
{
protected:
  int32 m_identifier;
  std::string m_name;
  LoadingStatus::Status m_loadingStatus;
  RenderSystem* m_renderDevice;

public:
  Resource( const std::string& name, RenderSystem* renderDevice );
  virtual ~Resource();

  int32 GetIdentifier() const;
  const std::string& GetName() const;
  bool IsLoaded() const;

  virtual bool Load( File* is );
  virtual bool Unload();

  /// Resets the loading status to Unloaded, allowing the resource to be re-initialized.
  /// This is used during error recovery rollback when the underlying impl has been
  /// destroyed but the Resource object itself is retained for retry.
  void ResetLoadingStatus();

private:
  virtual bool LoadImpl_( File* is ) = 0;
  virtual bool UnloadImpl_() = 0;

}; // Resource

} // namespace Nebulae

#endif // NEBULAE_ALPHA_RESOURCE_H__