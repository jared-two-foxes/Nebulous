
#include "Application.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

  
#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>


using namespace Nebulae;

  
Application::Application() 
///
/// Default Constructor
///
: m_bInitialized( false ),
  m_pPlatform( NULL ),
  m_pMainWindow( NULL ),
  m_pRenderSystem( NULL )
{}

Application::~Application()
///
/// Destructor.  Destroys all the subsystems that were created for running of application.
///
{
  if( IsInitialized() ) 
  {
    Destroy();
  }
}
  
void 
Application::Initiate( int w, int h )
///
/// Initiate required application resources.
///
/// @return
///   Nothing.
///
{
  /// Creates the platform object.
  CreatePlatform();

  m_logger.Register( std::cout, false );

  /// Create the main application sWindow and apply RenderSystem.
  Setup( w, h );

  // Flag Application as being initialized
  m_bInitialized = true;
}
  
void 
Application::Destroy()
///
/// Clean up the application resources.
///
/// @return
///   Nothing.
///
{
  m_pRenderSystem.reset();
  m_pPlatform->DestroyWindow( m_pMainWindow );
  m_pMainWindow.reset();
  m_pPlatform.reset();
  
  // Flag Application as not being initialized
  m_bInitialized = false;
}

void
Application::Run()
/// 
/// Begins the applications execution.  Uses pInitialState as the application state
/// upon start.
/// 
/// @param pInitialState
///   The application state that we wish to use when the app begins executing.
/// @return
///   Nothing.
///
{
  if( !IsInitialized() ) {
    Initiate( NULL );
  }

#if NE_PLATFORM == NE_PLATFORM_WIN32
  // Start the render loop
  StartGameLoop();  
  Destroy();
#endif
}
  
void 
Application::StartGameLoop()
///
/// Enters a loop which checks the platform object for any system messages until the 
/// application is closed.
///
/// @return
///   Nothing
///
/// @note
///   Assumes that only Win32 is executing through this function.
///
{
  uint64 lastProcessedUpTime = m_pPlatform->GetClock().GetUpTime();

  // Start loop
  while( !m_pPlatform->ShouldExit() ) {
    // Check message pump, quit if exit message was received.
    if( m_pPlatform->MessagePump() ) {
      m_pPlatform->Exit( 1 );
    }

    m_pPlatform->GetClock().MarkThisTick();

#if NE_PLATFORM == NE_PLATFORM_WIN32
    uint64 timeNow       = m_pPlatform->GetClock().GetUpTime();
    uint64 diff          = timeNow - lastProcessedUpTime;
    lastProcessedUpTime += ProcessApplicationLoop( diff );
#endif
  }
}

uint64 
Application::ProcessApplicationLoop( uint64 elapsed )
{
  BROFILER_FRAME( "ThreadName" )

  uint64 updatedTime = 0;

  // Do the rendering.
  m_pRenderSystem->Clear();

  ProcessFrame( elapsed );

  while( elapsed - updatedTime > 33333ULL )
  {
    ProcessSimulation( 33333ULL );
    updatedTime += 33333ULL;
  }

  // Present back buffer.
  m_pRenderSystem->SwapBuffers();
  
  m_logger.Flush();

  return updatedTime;
}

void 
Application::ProcessSimulation( uint64 elapsed )
///
/// Called each loop iteration a number of times with a fixed time step and contains the 
/// core logic for the application.
///
/// @param fTimeElapsed  
///   The time since the function was last called.
///
/// @return
///   Nothing.
/// 
{}

void 
Application::ProcessFrame( uint64 elapsed )
///
/// Called each frame and should be used to render the current snapshot of the applications
/// visual state.
///
/// @param fTimeElapsed  
///   The time since the function was last called.
///
/// @return
///   Nothing.
/// 
{}

Application::PlatformPtr
Application::CreatePlatform()
///
/// Creates the platform object for which all "native" platform functions are dispatched to.
///
/// @return
///   The Platform created.
/// 
{
	m_pPlatform = CreateAndInitiatePlatform();
	if ( m_pPlatform )
	{
		m_pPlatform->Initiate();

//#if !defined(USE_ZIPDEVICE_AS_DEFAULT_ROOT)
//		m_pPlatform->GetFileSystem()->Mount( "disk", new DiskFileDevice("..\\..\\UncompressedAssets") );
//#else
//		m_pPlatform->GetFileSystem()->Mount( "zip", new ZipFileDevice("..\\..\\Assets", "Assets") );
//#endif

    m_pPlatform->GetFileSystem()->Mount( "disk", new DiskFileDevice( "..\\..\\Samples\\Application" ) );
	}

	return m_pPlatform;
}

void 
Application::Setup( int w, int h )
///
/// Sets up all of the sub systems being required to run the Application. 
///
{
  NE_ASSERT( m_pPlatform != NULL, "Platform is uninitialized." )();

  // Create the window
  if( m_pMainWindow == NULL ) {
    m_pMainWindow = CreateApplicationWindow( w, h );
    NE_ASSERT( m_pMainWindow != NULL, "Unable to create application window" )();
    if( m_pMainWindow == NULL ) {
      return; // Crap! Window creation failed, exit.
    }
  }
    
  // Make the main window visible.
  m_pMainWindow->Show();

  // Create RenderSystem for the application
  m_pRenderSystem = ::CreateRenderSystem( OPENGL_3, m_pPlatform->GetFileSystem(), m_pMainWindow );
  if( m_pRenderSystem == NULL ) {
    m_pMainWindow.reset();
    return;
  }

	m_pRenderSystem->Initiate();
}

Application::WindowPtr
Application::CreateApplicationWindow( int w, int h )
///
/// Creates and returns a Window.
///
/// @todo [jared.watt 14/02/2013] 
///   Currently initialized to default values.  Need to be initialized to the device dimensions if no sizes are specified..
///
{      
  //@todo Query the platform object to determine optimum default dimensions!
  if( w == -1 ) {
    w = 640;
  }
  if( h == -1 ) {
    h = 960; 
  }

  return m_pPlatform->CreateApplicationWindow( 0, 0, w, h );
}

void
Application::SetMainWindow( WindowPtr window )
///
/// Sets the main application window.
///
/// @param window
///   The Window to use as the main application window.
///
/// @return
///   Nothing.
///
{
  if( m_pMainWindow ) {
    m_pMainWindow.reset();
  }
  m_pMainWindow = window;
}

void 
Application::AddInputListener( InputListener* listener )
///
/// Adds the listener to the main windows list of InputListeners.
///
/// @param listener
///   The InputListener to register with the main application. 
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_pMainWindow != NULL, "Main window not yet created" )();
  NE_ASSERT( listener != NULL, "Invalid listener." )();
    
  if( m_pMainWindow != NULL ) 
  {
    m_pMainWindow->AddInputListener( listener );
  }
}

void 
Application::RemoveInputListener( InputListener* listener )
///
/// Removes /param listener from the listener list of the application's main window.
///
{
  NE_ASSERT( m_pMainWindow != NULL, "Main window not yet created" )();
  NE_ASSERT( listener != NULL, "Invalid listener." )();
    
  if( m_pMainWindow != NULL ) 
  {
    m_pMainWindow->RemoveInputListener( listener );
  }
}

bool            
Application::IsInitialized() const   
{ return m_bInitialized; }

Application::PlatformPtr     
Application::GetPlatform() const     
{ return m_pPlatform; }

Application::RenderSystemPtr 
Application::GetRenderSystem() const 
{ return m_pRenderSystem; }

bool
Application::HasInputListener( InputListener* listener ) const
///
/// Checks the main window if it has a specified InputListener.
///
{
  NE_ASSERT( m_pMainWindow != NULL, "Main window not yet created" )();
    
  if( m_pMainWindow ) {
    return m_pMainWindow->HasInputListener( listener );
  }

  return false;
}