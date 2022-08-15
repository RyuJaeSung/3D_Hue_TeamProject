#pragma once

#ifndef __VIBUFFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeTexture final : public CVIBuffer
{
public:
	explicit CVIBuffer_CubeTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_CubeTexture(const CVIBuffer_CubeTexture& other);
	virtual ~CVIBuffer_CubeTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_CubeTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
};

END

#define __VIBUFFER_CUBETEXTURE_H__
#endif