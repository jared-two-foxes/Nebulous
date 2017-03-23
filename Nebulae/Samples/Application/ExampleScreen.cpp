
#include "ExampleScreen.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/Window.h>
//#include <Nebulae/Common/FileSystem/FileSystemArchive.h>

#include <Nebulae/Alpha/Shared/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/Shared/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/Shared/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/Shared/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shared/Shaders/UniformDefinition.h>

#include <Nebulae/Beta/Application/Application.h>
#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/StateStack/StateStack.h>


using namespace Nebulae;

Real gCubeVertexData_v2[216] =
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

const Real Pi                = 3.1415926536f;
const Real gDegreesToRadians = 0.0174532925f; // PI/180.0f
const Real gRadiansToDegrees = 57.2957795128f; // 180.0f/PI

ExampleScreen::ExampleScreen( const std::string& name, RenderSystem* renderSystem ) 
  : State( name ),
    m_camera( NULL ), 
    m_renderSystem( renderSystem ), 
    m_rotation( 0.0f ),
    m_material( NULL )
{

}


ExampleScreen::~ExampleScreen()
{

}


void 
ExampleScreen::Enter( StateStack* caller )
{
//
// Setup camera.
//
  //Vector4 vCameraEye( 0.75f, 0.0f, 0.75f );
  Vector4 vCameraEye( 0.0f, 0.0f, 2.0f );
  Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
  Vector4 vUp( 0.0f, 1.0f, 0.0f );
  
  m_camera = new Camera();
  m_camera->LookAt( vCameraEye, vLookAt, vUp );
  //m_camera->SetOrtho( 2.0f, 3.0f, 0.1f, 100.0f );
  Real aspect = fabsf(640.f / 960.f);
  m_camera->SetPerspective( gDegreesToRadians*90.0f, aspect, 0.1f, 1000.0f );

  HardwareBuffer* buffer = m_renderSystem->FindBufferByName( "CubeBuffer" );
  if( NULL == buffer )
  {
    // Unable to find an existing buffer, create a new one.
    buffer = m_renderSystem->CreateBuffer(
      "CubeBuffer", HardwareBufferUsage::HBU_STATIC, sizeof(gCubeVertexData_v2),
      HardwareBufferBinding::HBB_VERTEX, (void*)&gCubeVertexData_v2[0] );
  }

//
// Setup the material for rendering.  
//
  HardwareShader* vertexShader = m_renderSystem->CreateShader( "Shaders//Shader.vsh", VERTEX_SHADER );
  HardwareShader* pixelShader  = m_renderSystem->CreateShader( "Shaders//Shader.fsh", PIXEL_SHADER );

  m_material = new Material( "Materials//simple.material" ); 
  Pass* pass = m_material->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );
  
  // Attempt to grab the input layout
  InputLayout*   inputLayout = m_renderSystem->FindInputLayoutByName( "CubeLayout" );
  if( NULL == inputLayout )
  {
    // @todo [jared.watt 25.05.2013] Leaks. Needs to be deleted somewhere.
    VertexDeceleration* vertexDecl = new VertexDeceleration( 2 );
    vertexDecl->AddElement( VET_FLOAT3, VES_POSITION, 0 );
    vertexDecl->AddElement( VET_FLOAT3, VES_NORMAL, 0 );

    // Create the layout based upon the materials first pass.
    inputLayout = m_renderSystem->CreateInputLayout( "CubeLayout", vertexDecl );

    //@todo Delete the vertex deceleration objects.
  }
}


void 
ExampleScreen::Exit( StateStack* caller )
{

}


void 
ExampleScreen::Update( float fDeltaTimeStep, StateStack* pCaller )
{
  m_rotation += fDeltaTimeStep * 5.0f; 
  if( m_rotation > 360.0f ) {
    m_rotation -= 360.0f;
  }
} 


void
ExampleScreen::Render() const
{
  // Calculate the camera transforms.
  Matrix4 view = MatrixMakeFromOGLBuffer( m_camera->GetViewMatrix() );
  Matrix4 proj = MatrixMakeFromOGLBuffer( m_camera->GetProjectionMatrix() );

  {
    Matrix4 model;
    model.SetIdentity();
    model *= MatrixMakeTranslation( -1.5f, 0, 0 );
    model *= MatrixMakeRotation( gDegreesToRadians*m_rotation, Vector4(0,1,0) );

    Matrix4 mv  = view * model;
    Matrix4 mvp = proj * view * model;

    Vector4 diffuse(1.0f, 0.4f, 0.4f, 1.0f);

    DrawCube( mvp, mv, diffuse );
  }

  {
    Matrix4 model;
    model.SetIdentity();
    model *= MatrixMakeTranslation( 1.5f, 0, 0 );
    model *= MatrixMakeRotation( -gDegreesToRadians*m_rotation, Vector4(0,1,0) );

    Matrix4 mv  = view * model;
    Matrix4 mvp = proj * view * model;

    Vector4 diffuse(0.4f, 1.0f, 0.4f, 1.0f);

    DrawCube( mvp, mv, diffuse );
  }

}

void 
ExampleScreen::DrawCube( Matrix4& mvp, Matrix4& normal, Vector4& diffuseColour ) const
{
//
// Setup the render pass and push vertices.
//
  if( m_material )
  {
    // Grab the pass for the material (there is only one for now).
    Pass* pass = m_material->GetPass( 0 );

    // Set the operation type
    m_renderSystem->SetOperationType( OT_TRIANGLES );

    // Set the pass to the render system. 
    m_renderSystem->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

    // Set the Vertex Buffer
    std::size_t     offset = 0;
    std::size_t     stride = 6*sizeof(Real);
    HardwareBuffer* buffer = m_renderSystem->FindBufferByName( "CubeBuffer" );
    if( buffer ) {
      m_renderSystem->SetVertexBuffers( 0, buffer->GetImpl(), stride, offset );
    }

    // Set the Vertex input layout
    InputLayout* inputLayout = m_renderSystem->FindInputLayoutByName( "CubeLayout" );
    if( inputLayout ) {
      m_renderSystem->SetInputLayout( inputLayout->GetImpl() );
    }

    // Create projection variable from desc.
    UniformDefinition worldVarDef   = m_renderSystem->GetUniformByName( "modelViewProjectionMatrix" );
    UniformDefinition normalVarDef  = m_renderSystem->GetUniformByName( "normalMatrix" );
    UniformDefinition diffuseVarDef = m_renderSystem->GetUniformByName( "diffuseColor" );

    Real mvpBuffer[16];
    MatrixFillOGLBuffer( mvp, &mvpBuffer[0] );

    Real normalBuffer[16];
    MatrixFillOGLBuffer( normal, &normalBuffer[0] );

    m_renderSystem->SetUniformBinding( worldVarDef, (void*)&mvpBuffer[0] );
    m_renderSystem->SetUniformBinding( normalVarDef, (void*)&normalBuffer[0] );
    m_renderSystem->SetUniformBinding( diffuseVarDef, (void*)&diffuseColour[0] );

    // Draw. 
    m_renderSystem->Draw( 36, 0 );
  }
}