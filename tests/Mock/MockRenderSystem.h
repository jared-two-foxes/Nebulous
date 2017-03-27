#ifndef __BETA_TESTS_MOCKRENDERDEVICE_H__
#define __BETA_TESTS_MOCKRENDERDEVICE_H__

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include "gmock/gmock.h"

namespace Nebulae {

class MockRenderDevice : public RenderSystem
{
private:
	std::string m_name;

  public:
    MockRenderDevice( FileSystemPtr fileSystem, WindowPtr window ) : RenderSystem(fileSystem,window), m_name("gles") {}
    
    virtual const std::string& GetName() const { return m_name; }

    MOCK_METHOD0( Clear, void () );

    //factory functions.
    MOCK_METHOD2( CreateInputLayoutImpl, InputLayoutImpl* ( VertexDeceleration* pVertexDecl, HardwareShader* pVertexShader ) );
    MOCK_METHOD4( CreateBufferImpl, HardwareBufferImpl* ( HardwareBufferUsage usage, size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem ) );
    MOCK_METHOD2( CreateShaderImpl, HardwareShaderImpl* ( const std::string& strFileName, HardwareShaderType eType ) );
    MOCK_METHOD0( CreateSamplerImpl, Sampler::Impl* () );
    MOCK_METHOD1( CreateTextureImpl, TextureImpl* ( const std::string& strFileName ) );

    MOCK_CONST_METHOD1( GetUniformByName, UniformDefinition ( const char* ) );

    //pipeline functions.
    MOCK_METHOD4( SetVertexBuffers, void (int, HardwareBuffer*, size_t, size_t) );
    MOCK_METHOD1( SetInputLayout, void (InputLayoutImpl*) );
    MOCK_METHOD2( SetIndexBuffer, void(HardwareBuffer*, size_t) );
    MOCK_METHOD2( SetShaders, void( HardwareShader*, HardwareShader*) );
    MOCK_METHOD1( SetOperationType, void(OperationType) );
    MOCK_METHOD2( Draw, void(int, int) );
    MOCK_METHOD3( DrawIndexed, void(int, int, int) );


    //shader binding functions.
    MOCK_METHOD3( SetBufferBinding, void(uint32, uint32, HardwareBuffer*) );
    MOCK_METHOD3( SetSamplerBinding, void(uint32, uint32, Sampler::Impl*) );
    MOCK_METHOD3( SetTextureBinding, void(uint32, uint32, Texture*) );
    MOCK_METHOD2( SetUniformBinding, void(UniformDefinition&, void*) );

};

} //Nebulae

#endif // __BETA_TESTS_MOCKRENDERDEVICE_H__