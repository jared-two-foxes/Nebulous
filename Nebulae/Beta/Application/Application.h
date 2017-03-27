#ifndef __NEBULAE_APPLICATION_H__
#define __NEBULAE_APPLICATION_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{ 
  class InputListener;
  class Platform;
  class RenderSystem;
  class Window;

  /** Encapsulates the life of the entire application and acts as a container/manager 
   *  for all of the major engine subsystems that are used for the life of the 
   *  application.
   */
  class Application
  {
  public:
    typedef std::shared_ptr<Platform >     PlatformPtr;
    typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
    typedef std::shared_ptr<Window >       WindowPtr;

  protected:
    bool            m_bInitialized;    ///< Indicates if the application has been "initiated"
    PlatformPtr     m_pPlatform;       ///< The object defining the platform that application is running on. Strong Reference. 
    WindowPtr       m_pMainWindow;     ///< The main application window. Strong Reference.
    RenderSystemPtr m_pRenderSystem;   ///< The context used for rendering to the application window. Strong Reference.
    uint64          m_currentFrame;    ///< Records the current number of frames processed.
    Logger          m_logger;          ///< Create Logger object.  (Can be accessed after creation through singleton interface).
    float           m_framesPerSecond; ///< The number of frames that are being rendered per second.
       
    public:
      /** \name Structors */ ///@{
      Application();
      virtual ~Application();
      //@}

      /** \name Accessors */ ///@{
      bool            IsInitialized() const;
      PlatformPtr     GetPlatform() const;
      RenderSystemPtr GetRenderSystem() const;
      bool            HasInputListener( InputListener* listener ) const;
      //@}

      /** \name Mutators */ ///@{
      virtual void Initiate( int w = -1, int h = -1 );
      virtual void Destroy();
      virtual void Run();
      virtual uint64 ProcessApplicationLoop( uint64 elapsed );
      virtual void ProcessSimulation( uint64 elapsed );
      virtual void ProcessFrame( uint64 elapsed );
      void SetMainWindow( WindowPtr window );
      void AddInputListener( InputListener* listener );
      void RemoveInputListener( InputListener* listener );
      //@}
      

    private:
      /** \name Mutators */ ///@{
      virtual void        StartGameLoop();
      virtual PlatformPtr CreatePlatform();
      virtual void        Setup( int w, int h );
      virtual WindowPtr   CreateApplicationWindow( int w, int h );
      //@}

  }; //Application

} //Nebulae

#endif // __NEBULAE_APPLICATION_H__