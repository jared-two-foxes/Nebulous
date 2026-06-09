#ifndef NEBULAE_MANIFESTDISKFILEDEVICE_H__
#define NEBULAE_MANIFESTDISKFILEDEVICE_H__

#include <Nebulae/Common/Common.h>
#include <map>

namespace Nebulae
{

class DiskFileDevice;

class ManifestDiskFileDevice : public FileDevice
{
public:
  ManifestDiskFileDevice( const char* rootDirectory );
  virtual ~ManifestDiskFileDevice();

  virtual File* Open( const std::string& path, FileSystem::Mode mode ) override;
  virtual File* Open( File* file ) override;
  virtual void Close( File* file ) override;

  /// Resolve a runfile filename to its absolute path using the Bazel runfiles manifest.
  static std::string ResolveRunfile( const std::string& filename );

private:
  void LoadManifest();
  void LoadManifestFromFile( const std::string& filepath, std::map<std::string, std::string>& map );

  DiskFileDevice* m_fallback;
  std::string m_rootDirectory;
  std::map<std::string, std::string> m_manifestMap;
  bool m_manifestLoaded;
};

} // namespace Nebulae

#endif // NEBULAE_MANIFESTDISKFILEDEVICE_H__
