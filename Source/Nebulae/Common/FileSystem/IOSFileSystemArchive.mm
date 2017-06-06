
#include "IOSFileSystemArchive.h"

#include "Common.h"

#ifdef __OBJC__
  #import <UIKit/UIKit.h>
#endif

using namespace Nebulae;


IOSFileSystemArchive::IOSFileSystemArchive()
  : FileSystemArchive()
{

}

const boost::filesystem::path
IOSFileSystemArchive::CreateAbsolutePath( const std::string& filename ) const
{
  //@note. Files are copied into the application bundle "flat", that is with no folder
  // structure.  Also they are queried from the bundle using just the file name and
  // extension seperate.
  
  boost::filesystem::path p( filename );
  boost::filesystem::path stem = p.stem();
  boost::filesystem::path ext  = p.extension();
  
  //NSString* stemPart = [NSString stringWithCString:stem.string().c_str() encoding:[NSString defaultCStringEncoding]];
  NSString* extPart  = [NSString stringWithUTF8String:ext.string().c_str()];
  NSString* stemPart = [NSString stringWithUTF8String:stem.string().c_str()];
  NSURL*    url      = [[NSBundle mainBundle] URLForResource:stemPart withExtension:extPart ];
  
  return nil == url ? p : boost::filesystem::path( [url.path UTF8String] );
}