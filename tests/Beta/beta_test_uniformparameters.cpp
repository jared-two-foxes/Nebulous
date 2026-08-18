#include "gtest/gtest.h"

#include <filesystem>

namespace
{
std::filesystem::path WorkspaceRoot()
{
  std::filesystem::path sourcePath( __FILE__ );
  if ( sourcePath.is_relative() )
    sourcePath = std::filesystem::absolute( sourcePath );

  for ( std::filesystem::path path = sourcePath.parent_path(); !path.empty(); path = path.parent_path() )
  {
    if ( std::filesystem::exists( path / "MODULE.bazel" ) )
      return path;

    if ( path == path.parent_path() )
      break;
  }

  return {};
}
} // namespace

TEST( UniformParameters, LegacyFilesAreRemoved )
{
  const std::filesystem::path root = WorkspaceRoot();
  ASSERT_FALSE( root.empty() ) << "Could not locate workspace root from the test source path.";

  EXPECT_FALSE( std::filesystem::exists( root / "Source/Nebulae/Beta/Scene/UniformParameters.h" ) );
  EXPECT_FALSE( std::filesystem::exists( root / "Source/Nebulae/Beta/Scene/UniformParameters.cpp" ) );
}
