#ifndef __INPUTLAYOUTIMPL_OGL_H__
#define __INPUTLAYOUTIMPL_OGL_H__

#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>


namespace Nebulae {

class ProgramObject;

class InputLayoutImpl_OGL : public InputLayoutImpl
{
public:
	InputLayoutImpl_OGL( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
	virtual ~InputLayoutImpl_OGL();

	virtual bool Load() override;
	void         Bind( ProgramObject* programObject );
	static void  initiateFunctions();

};

}

#endif // __INPUTLAYOUTIMPL_OGL_H__