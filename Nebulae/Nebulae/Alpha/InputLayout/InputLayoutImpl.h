#ifndef __NEBULAE_ALPHA_INPUTLAYOUTIMPL_H__
#define __NEBULAE_ALPHA_INPUTLAYOUTIMPL_H__

/// #include <Nebulae/Common/Common.h>

namespace Nebulae {

class HardwareShader;
class VertexDeceleration;

class InputLayoutImpl
{
protected:  
  const VertexDeceleration* m_vertexDecl;
  const HardwareShader*     m_vertexShader;

  public:
    InputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
    virtual ~InputLayoutImpl();

    const VertexDeceleration* GetVertexDecleration() const;
    const HardwareShader*     GetVertexShader() const;

    virtual bool Load();
    virtual bool Unload();
};

}

#endif // __NEBULAE_ALPHA_INPUTLAYOUTIMPL_H__