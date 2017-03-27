
#include "SimpleState.h"

#include <Nebulae/Common/Window/Window.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/Buffer/HardwareBufferManager.h>
#include <Nebulae/Alpha/Camera/Camera.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutManager.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/Material/Material.h>
#include <Nebulae/Alpha/Material/MaterialManager.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/RenderSystem/UniformVariable.h>

#include <Nebulae/Beta/Debug/DebugUtil.h>
#include <Nebulae/Beta/StateStack/StateStack.h>

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

    // Setup camera
    m_pCamera = new Camera( m_pRenderSystem );
    Vector4 vCameraEye( 0.0f, 0.0f, 5.0f );
    Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
    Vector4 vUp( 0.0f, 1.0f, 0.0f );
    m_pCamera->LookAt( vCameraEye, vLookAt, vUp );
    m_pCamera->SetOrtho();


    m_debugRenderer = new DebugUtil( m_pRenderSystem );
    m_debugRenderer->Init();

#if 0
    // setup the material for rendering gui components.
    Material* pMaterial = MaterialManager::getSingletonPtr()->GetByName( "materials//debug.material" ); 
    if( pMaterial == NULL )
    {
      pMaterial = MaterialManager::getSingletonPtr()->Create( "materials//debug.material" ); 
      assert( pMaterial );
      if( pMaterial ) {
        pMaterial->Load();
      }
    }

    static const Real fVertices[] = { 2.0f,2.0f,0.0f, -2.0f,-2.0f,0.0f };

    HardwareBuffer* pBuffer = HardwareBufferManager::getSingletonPtr()->GetByName( "DebugBuffer" );
    if( pBuffer == NULL )
    {
      // Unable to find an existing buffer, create a new one.
      pBuffer = HardwareBufferManager::getSingleton().Create(
        "DebugBuffer", NULL, HardwareBufferUsage::HBU_STATIC, 6*sizeof(Real), 
        HardwareBufferBinding::HBB_VERTEX, (void*)&fVertices[0]);
    }

    // Attempt to grab the input layout
    InputLayout* pInputLayout = InputLayoutManager::getSingletonPtr()->GetByName( "DebugLayout" );
    if( pInputLayout == NULL )
    {
      // Unable to find an existing layout, create a new one.

      // @todo [jared.watt 25.05.2013] Leaks. Needs to be deleted somewhere.
      VertexDeceleration* pVertexDecl = new VertexDeceleration( 2 );
      pVertexDecl->AddElement( VET_FLOAT3, VES_POSITION, 0 );

      // Create the layout based upon the materials first pass.
      pInputLayout = InputLayoutManager::getSingletonPtr()->Create( "DebugLayout", NULL, pMaterial->GetPass(0), pVertexDecl );
      assert( pInputLayout );
      if( pInputLayout ) {
        pInputLayout->Load();
      }
    }
#endif
  }

  void 
  SimpleState::Exit( Nebulae::StateStack* caller )
  {
    if( m_pCamera ) {
      delete m_pCamera;
      m_pCamera = NULL;
    }
  }

  void 
  SimpleState::Update( float fDeltaTimeStep, Nebulae::StateStack* pCaller )
  {

  } 

  void
  SimpleState::Render() const
  {
    m_debugRenderer->Reset();
    m_debugRenderer->AddLine( Vector4(2.0f,2.0f,0.0f), Vector4(-2.0f,-2.0f,0.0f), Vector4(1.0,1.0,1.0,1.0) ); 
    m_debugRenderer->Draw( m_pCamera );

#if 0
    // Grab the material that we are using.
    Material* pMaterial = MaterialManager::getSingletonPtr()->GetByName( "materials//debug.material" ); 
    assert( pMaterial );
    if( pMaterial )
    {
      // Grab the pass for the material (there is only one).
      Pass* pPass = pMaterial->GetPass( 0 );

      // Set the pass to the render system. 
      m_pRenderSystem->SetPass( pPass->GetImpl() );

      // Set the Vertex Buffer
      size_t iOffset = 0;
      size_t iStride = 3*sizeof(Real);
      HardwareBuffer* pBuffer = HardwareBufferManager::getSingletonPtr()->GetByName( "DebugBuffer" );
      HardwareBuffer::Impl* pVertexBuffer = pBuffer->getImpl();
      m_pRenderSystem->SetVertexBuffers( 0, pVertexBuffer, iStride, iOffset );

      // Set the Vertex input layout
      InputLayout* pInputLayout = InputLayoutManager::getSingletonPtr()->GetByName( "DebugLayout" );
      m_pRenderSystem->SetInputLayout( pInputLayout->getImpl() );
      
      // Set the operation type
      m_pRenderSystem->SetOperationType( OT_LINES );


      // Create projection variable from desc.
      PassVariable* pWorldVarDesc       = pPass->GetVariableByName( "world" );
      PassVariable* pViewVarDesc        = pPass->GetVariableByName( "view" );
      PassVariable* pProjectionVarDesc  = pPass->GetVariableByName( "projection" );

    //
    // Calculate the local transform of the particle
    //
      Matrix4 worldMatrix;
      worldMatrix.SetIdentity();

    //
    // Set camera transforms for pass
    //
      UniformVariable worldVar( pWorldVarDesc, (void*)worldMatrix.Get() );
      UniformVariable viewVar( pViewVarDesc, (void*)m_pCamera->GetViewMatrix() );
      UniformVariable projectionVar( pProjectionVarDesc, (void*)m_pCamera->GetProjectionMatrix() );

      m_pRenderSystem->SetUniformBinding( &worldVar );
      m_pRenderSystem->SetUniformBinding( &viewVar );
      m_pRenderSystem->SetUniformBinding( &projectionVar );

    //
    // Draw particle.
    //
      m_pRenderSystem->Draw( 2, 0 );
    }
#endif
  }

}