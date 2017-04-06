
#include "SimpleState.h"

#include <Nebulae/Common/Window/Window.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/StateStack/StateStack.h>
//#include <Nebulae/Beta/Material/Material.h>
//#include <Nebulae/Beta/Scene/UniformParameters.h>

using namespace Nebulae;

namespace Sample
{

  SimpleState::SimpleState( const std::string& name ) 
    : State( name )  
    , m_pCamera( NULL )
  {

  }

  SimpleState::~SimpleState()
  {

  }

  void
  SimpleState::Enter( Nebulae::StateStack* caller )
  {
    // Grab Application variables to help with setup.
    m_pRenderSystem = caller->GetRenderSystem();

    // Grab the window object
    RenderSystem::WindowPtr window = m_pRenderSystem->GetWindow();

    // Setup camera
    m_pCamera = std::make_shared<Camera >();
    Vector4 vCameraEye( 0.0f, 0.0f, 5.0f );
    Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
    Vector4 vUp( 0.0f, 1.0f, 0.0f );
    m_pCamera->LookAt( vCameraEye, vLookAt, vUp );
    m_pCamera->SetOrtho( window->GetWidth(), window->GetHeight(), 0.1f, 1000.0f );


    static const Real fVertices[] = { 
      -50.0f,0.0f,0.0f, 1.0f,1.0f,1.0f,1.0f,1.0f, 
      50.0f,0.0f,0.0f, 1.0f,1.0f,1.0f,1.0f,1.0f 
    };

    // Attempt to create a Buffer of video memory
    HardwareBuffer* pBuffer = m_pRenderSystem->CreateBuffer( 
      "DebugBuffer", 
      HBU_STATIC,
      14*sizeof(Real), 
      HardwareBufferBinding::HBB_VERTEX, 
      (void*)&fVertices[0] );

    // Attempt to create a vertex shader.
    HardwareShader* pVertexShader = m_pRenderSystem->CreateShader( 
      "debug_vs.glsl", VERTEX_SHADER ); 

    // Attempt to create a fragment shader.
    HardwareShader* pPixelShader = m_pRenderSystem->CreateShader( 
      "debug_fs.glsl", PIXEL_SHADER ); 

    // @todo [jared.watt 25.05.2013] Leaks. Needs to be deleted somewhere.
    VertexDeceleration* pVertexDecl = new VertexDeceleration( 3 );
    pVertexDecl->AddElement( VET_FLOAT3, VES_POSITION, 0 );
    pVertexDecl->AddElement( VET_FLOAT4, VES_DIFFUSE, 0 );

    // Attempt to create an input layout
    InputLayout* pInputLayout = m_pRenderSystem->CreateInputLayout( 
      "DebugLayout", pVertexDecl, pVertexShader );
  }

  void 
  SimpleState::Exit( Nebulae::StateStack* caller )
  {

  }

  void 
  SimpleState::Update( float fDeltaTimeStep, Nebulae::StateStack* pCaller )
  {

  } 

  void
  SimpleState::Render() const
  {
    // Set the operation type
    m_pRenderSystem->SetOperationType( OT_LINES );

    // Set the Vertex Buffer
    size_t iOffset = 0;
    size_t iStride = 7*sizeof(Real);
    HardwareBuffer* pBuffer = m_pRenderSystem->FindBufferByName( "DebugBuffer" );
    m_pRenderSystem->SetVertexBuffers( 0, pBuffer, iStride, iOffset );

    HardwareShader* pVertexShader = m_pRenderSystem->FindShaderByName( "debug_vs.glsl" );
    HardwareShader* pPixelShader = m_pRenderSystem->FindShaderByName( "debug_fs.glsl" );
    m_pRenderSystem->SetShaders( pVertexShader, pPixelShader );

    // Set the Vertex input layout
    InputLayout* pInputLayout = m_pRenderSystem->FindInputLayoutByName("DebugLayout");
    m_pRenderSystem->SetInputLayout(pInputLayout);

    // Create projection variable from desc.
    UniformDefinition& worldVarDesc       = m_pRenderSystem->GetUniformByName( "world" );
    UniformDefinition& viewVarDesc        = m_pRenderSystem->GetUniformByName( "view" );
    UniformDefinition& projectionVarDesc  = m_pRenderSystem->GetUniformByName( "projection" );

    // Calculate the local transform of the particle
    Matrix4 worldMatrix;
    worldMatrix.SetIdentity();

    float world[16], view[16], projection[16];
    worldMatrix.GetOpenGL( &world[0] );
    m_pCamera->GetViewMatrix().GetOpenGL( &view[0] );
    m_pCamera->GetProjectionMatrix().GetOpenGL( &projection[0] );

    // Set camera transforms for pass
    m_pRenderSystem->SetUniformBinding( worldVarDesc, (void*)&world[0] );
    m_pRenderSystem->SetUniformBinding( viewVarDesc, (void*)&view[0] );
    m_pRenderSystem->SetUniformBinding( projectionVarDesc, (void*)&projection[0] );

    // Draw particle.
    m_pRenderSystem->Draw( 2, 0 );
  }

}