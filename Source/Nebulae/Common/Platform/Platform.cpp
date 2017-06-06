#include "Platform.h"

//#include <Nebulae/Alpha/Alpha.h>


using namespace Nebulae;


Platform::Platform()
///
/// Default constructor.
///
  : m_iExitCode( 0 ), 
    m_fileSystem( NULL )
{}

	
Platform::~Platform()
///
/// Default destructor.
///
{}

	
void 
Platform::Exit( int iExitCode )
///
///	Sets a flag indicating the state of the application when it exists.
///
/// @param iExitCode
///		The exit code that we wish to set.
///
/// @return
/// 	Nothing.
///
{
	m_iExitCode = iExitCode;
}


void 
Platform::SetWorkingDirectory( const char* directory )
///
/// Sets the working directory of the application.  Virtual function that doesnt nothing.
///
/// @param directory
///   Path to the directory to use.
///
/// @return 
///   Nothing.
///
/// @todo [jared.watt 13.02.2014]
///   Should this dispatch through to the FileSystem.  Would argue hell yes. And this function could possibly
///   just disappear from here.
///
{}


int                
Platform::GetExitCode() const 
///
/// Returns the value that which is returned at Application exit.
///
/// @return
///   The value.
///
{ return m_iExitCode; }


bool               
Platform::ShouldExit() const  
///
/// Returns if the Application should exit.
///
/// @return
///   Should the application end.
///
{ return (m_iExitCode != 0); }


Platform::FileSystemPtr
Platform::GetFileSystem()
///
/// Retrieves the FileSystem.
///
/// @return
///   The file system.
///
{ return m_fileSystem; }


Clock&  
Platform::GetClock()
///
/// Returns the platforms Clock object.
///
/// @return
///   The Clock object.
///
{ return m_clock; }


Keyboard&     
Platform::GetKeyboard()
///
/// Returns the platforms Keyboard object.
///
/// @return
///   The Keyboard object.
///
{ return m_keyboard; }


//Platform::RenderSystemPtr
//Platform::CreateRenderSystem( WindowPtr window )
/////
///// Creates a RenderSystem for the specified Window object.
/////
///// @param window
/////   A shared_ptr to the Window object that the created RenderSystem should apply.
/////
///// @return
/////   The created RenderSystem.
/////
//{
//  // Create RenderSystem for the window parameter object. 
//  RenderSystemPtr renderSystem = ::CreateRenderSystem( GLES_2_0, m_fileSystem, window ); //< TODO resolve circle dependency here!
//  if( renderSystem ) {
//    renderSystem->Initiate();
//  }
//
//  // Return.
//  return renderSystem;
//}