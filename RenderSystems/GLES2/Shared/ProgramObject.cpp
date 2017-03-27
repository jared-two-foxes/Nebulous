
#include "ProgramObject.h"
#include "GLES2HardwareShaderImpl.h"

using namespace Nebulae;


UniformType
ConvertGLUnformTypeToNebulaeType( GLenum type )
{
  switch( type )
  {
    case GL_FLOAT:        return UT_FLOAT1;
    case GL_FLOAT_VEC2:   return UT_FLOAT2;
    case GL_FLOAT_VEC3:   return UT_FLOAT3;
    case GL_FLOAT_VEC4:   return UT_FLOAT4;
    case GL_INT:          return UT_INT1;
    case GL_INT_VEC2:     return UT_INT2;
    case GL_INT_VEC3:     return UT_INT3;
    case GL_INT_VEC4:     return UT_INT4;
    case GL_BOOL:         return UT_INT1;
    case GL_BOOL_VEC2:    return UT_INT2;
    case GL_BOOL_VEC3:    return UT_INT3;
    case GL_BOOL_VEC4:    return UT_INT4;
    case GL_FLOAT_MAT2:   return UT_MATRIX_2X2;
    case GL_FLOAT_MAT3:   return UT_MATRIX_3X3;
    case GL_FLOAT_MAT4:   return UT_MATRIX_4X4;
    case GL_SAMPLER_2D:   return UT_SAMPLER2D;
    case GL_SAMPLER_CUBE: return UT_SAMPLERCUBE; 
  }

  return UT_UNKNOWN;
}

//constructor
ProgramObject::ProgramObject( HardwareShader* vertexShader, HardwareShader* fragmentShader )
	: m_handle( 0 ),
    m_vertexShader( vertexShader ),
    m_fragmentShader( fragmentShader )
{}


//destructor
ProgramObject::~ProgramObject()
{
	if( m_handle != NULL ) {
		glDeleteProgram( m_handle );
	}
  m_handle = 0;
}


GLuint 
ProgramObject::GetHandle() const 
{ return m_handle; }


HardwareShader* 
ProgramObject::GetVertexShader() const
{ return m_vertexShader; }


HardwareShader* 
ProgramObject::GetFragmentShader() const
{ return m_fragmentShader; }


const UniformDefinition 
ProgramObject::FindUniformByName( const char* name ) const
{
  UniformDefinitionMap::const_iterator it = m_uniformDefinitions.find( name );
  if( it == m_uniformDefinitions.end() ) {
    return UniformDefinition();
  }

  return it->second;
}


bool
ProgramObject::Load()
{
  NE_ASSERT( m_vertexShader != NULL, "Vertex Shader is not set for program" )();
  NE_ASSERT( m_fragmentShader != NULL, "Fragment Shader is not set for program" )();

	m_handle = glCreateProgram();
	CheckForGLESError();

	// Attach the vertex shader.
	GLES2HardwareShaderImpl* shaderImpl = static_cast< GLES2HardwareShaderImpl* >( m_vertexShader->GetImpl() );
	NE_ASSERT( shaderImpl != NULL, "Vertex Shader does not have a valid Impl object" )( m_vertexShader );
	glAttachShader( m_handle, shaderImpl->GetHandle() );
  CheckForGLESError();

	// Attach the pixel shader.
	shaderImpl = static_cast< GLES2HardwareShaderImpl* >( m_fragmentShader->GetImpl() );
	NE_ASSERT( shaderImpl != NULL, "Fragment Shader does not have a valid Impl object" )( m_fragmentShader );
	glAttachShader( m_handle, shaderImpl->GetHandle() );
  CheckForGLESError();

	// Attempt to link the program.
	glLinkProgram( m_handle );

	// Check if link was successful.
	GLint iStatus;
	glGetProgramiv( m_handle, GL_LINK_STATUS, &iStatus );
	if( iStatus == GL_FALSE )
	{
		fprintf( stderr, "Failed to link Pass\n");

		GLint iLen;
		glGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &iLen );

		char* szLog = new char[iLen];
		glGetProgramInfoLog( m_handle, iLen, NULL, szLog );
		fprintf( stderr, "%s", szLog );
		delete [] szLog;

    return false;
	}

	// Since OpenGL isnt guarenteed to finalise the generation of the shader program until its 'bound' we will fake bind it now.
	glUseProgram( m_handle );
  CheckForGLESError();

//
// Populate the uniform definition map with active uniforms.
//
  int totalActiveUniforms = -1;
  glGetProgramiv( m_handle, GL_ACTIVE_UNIFORMS, &totalActiveUniforms ); 
  for( int i = 0; i < totalActiveUniforms; ++i ) {
    GLenum type    = GL_ZERO;
    GLint  nameLen = -1,
           num     = -1;
    char   name[100];

    glGetActiveUniform( m_handle, GLuint(i), sizeof(name)-1,
      &nameLen, &num, &type, name );

    name[nameLen]   = 0;

    GLuint location = glGetUniformLocation( m_handle, name );
    NE_ASSERT( location != -1, "Unable to find the location of uniform %s", name )();
 
    UniformDefinition definition;
    definition.type         = ConvertGLUnformTypeToNebulaeType( type );
    definition.logicalIndex = location;
    definition.elementSize  = 1;
    definition.arraySize    = num;

    m_uniformDefinitions.insert( std::pair<std::string,UniformDefinition>(name, definition) );
  }

	return true;
}


void 
ProgramObject::UseProgram()
{
	NE_ASSERT( m_handle != 0, "Handle for ProgramObject is invalid" )();
	glUseProgram( m_handle );
}