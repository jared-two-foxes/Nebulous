//
//  GLES2RenderSystem.h
//  Nebulae
//
//  Created by jessica mccormack on 6/02/13.
//  Copyright 2013 auckland university. All rights reserved.
//
#ifndef __NEBULAE_GLES2_RENDERSYSTEM_H__
#define __NEBULAE_GLES2_RENDERSYSTEM_H__

#include <Nebulae/Alpha/GLES2/config.h>
#include <Nebulae/Alpha/Shared/RenderSystem/RenderSystem.h>

#ifdef __OBJC__
#include <Nebulae/Alpha/GLES2/EAGL/EAGLViewController.h>
@class EAGLContext;
#endif

namespace Nebulae
{

class FileSystemArchive;
class ProgramObject;

typedef std::vector<ProgramObject* > ProgramObjectList;

/**
 *  GLES2RenderSystem.
 */
class GLES2RenderSystem : public RenderSystem
{
private:
  GLuint              m_defaultframebuffer;
  GLuint              m_colorrenderbuffer;
  GLuint              m_depthrenderbuffer;
  GLenum              m_operationMode;
	ProgramObject*      m_boundProgram;    ///< The current bound link program.
  ProgramObjectList   m_programObjects;
  HardwareShader*     m_vertexShader;
  HardwareShader*     m_fragmentShader;  

#ifdef __OBJC__
  EAGLViewController* m_pViewController;
  EAGLContext*        m_pContext;
#endif
      
  public:
    GLES2RenderSystem( FileSystemArchive& fileSystem, Window* pWindow );
    virtual ~GLES2RenderSystem();  

    virtual const std::string GetName() const;

    //functions
    virtual bool Initiate() override;
    virtual void Clear() override;
    virtual void SetClearColour( Real r, Real g, Real b, Real a ) override;
    virtual void SetBlendingState( bool enable ) override;
    virtual void SetDepthTest( bool enable ) override;
    virtual void SwapBuffers( bool bWaitForVsync = false ) override;


    //factory functions
    virtual InputLayout::Impl*  CreateInputLayoutImpl( VertexDeceleration* pVertexDecl ) override;
    virtual HardwareBufferImpl* CreateBufferImpl( HardwareBufferUsage usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem ) override;
    virtual HardwareShaderImpl* CreateShaderImpl( const std::string& fileName, HardwareShaderType type ) override;
    virtual Sampler::Impl*      CreateSamplerImpl() override;
    virtual TextureImpl*        CreateTextureImpl( const std::string& strFileName ) override;

    //pipeline functions
    virtual void SetVertexBuffers( int iSlot, /*int iCount,*/ HardwareBuffer* pBuffers, std::size_t iStride, std::size_t iOffset ) override;
    virtual void SetInputLayout( InputLayout::Impl* pInputLayout ) override;
    virtual void SetIndexBuffer( HardwareBuffer* pBuffer, uint32 iOffset ) override;
    virtual void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader ) override;
    virtual void SetOperationType( OperationType eType ) override;
    virtual void Draw( int iVertexCount, int iStartVertexLocation ) override;
    virtual void DrawIndexed( int iIndexCount, int iStartIndexLocation, int iBaseVertexLocation ) override;

    //shader binding functions
    virtual UniformDefinition GetUniformByName( const char* name ) const override;
    virtual void SetBufferBinding( uint32 iTarget, uint32 iCount, HardwareBuffer* pBuffer ) override;
    virtual void SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler::Impl* pImpl ) override;
    virtual void SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* pImpl ) override;
    virtual void SetUniformBinding( UniformDefinition& definition, void* value ) override;

  private:
    void CreateFramebuffer();
    void DestroyFramebuffer();


}; // GLES2RenderSystem
	
} // Nebulae

#endif // __NEBULAE_GLES2_RENDERSYSTEM_H__