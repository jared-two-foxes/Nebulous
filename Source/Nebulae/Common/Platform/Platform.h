#ifndef __NEBULAE_BASEPLATFORM_H__
#define __NEBULAE_BASEPLATFORM_H__

#include <Nebulae/Common/Platform/Clock.h>
#include <Nebulae/Common/Platform/Keyboard.h>

namespace Nebulae {

class FileSystem;
//class RenderSystem;
class Window;

/** 
  *  Encapsulates all interactions that are required with the native platform upon which
  *  the application is running on.  Any time we wish to request something from the system
  *  that functions MUST be dispatched through this class rather than calling the functions 
  *  natively from within application code.
  */
class Platform
{
public:
  typedef std::shared_ptr<FileSystem >   FileSystemPtr;
  //typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
  typedef std::shared_ptr<Window >       WindowPtr;

protected:
  int           m_iExitCode;  ///< The value in which the application is to return on exit.
  FileSystemPtr m_fileSystem; ///< File access system.
  Clock         m_clock;      ///< The platform clock.
  Keyboard      m_keyboard;   ///< The key state for all the keys on the keyboard.
  
  public:
    /** \name Structors */ ///@{
    Platform();
    virtual ~Platform();
    //@}

    /** \name Accessors */ ///@{
    int           GetExitCode() const;
    bool          ShouldExit() const;
    FileSystemPtr GetFileSystem();
    Clock&        GetClock();
    Keyboard&     GetKeyboard();
    //@}

    /** \name Mutators */ ///@{
    virtual bool Initiate() = 0;
    virtual void Destroy() = 0;
    virtual bool MessagePump() { return false; }
    virtual void Exit( int iExitCode );
    virtual void SetWorkingDirectory( const char* directory );
    //@}
    
    /** \name Factory Methods */ ///@{
    virtual WindowPtr       CreateApplicationWindow( int x, int y, int w, int h, WindowPtr pParent = NULL ) = 0;
    virtual void            DestroyWindow( WindowPtr window ) = 0;
    //virtual RenderSystemPtr CreateRenderSystem( WindowPtr pWindow );
    //@}

};

std::shared_ptr<Platform > CreateAndInitiatePlatform();

}

#endif // NEBULAE__