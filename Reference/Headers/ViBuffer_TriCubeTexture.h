#pragma once

#ifndef __VIBUFFER_TRICUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TriCubeTexture final : public CVIBuffer
{
public:
	explicit CVIBuffer_TriCubeTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_TriCubeTexture(const CVIBuffer_TriCubeTexture& other);
	virtual ~CVIBuffer_TriCubeTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_TriCubeTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
};

END

#define __VIBUFFER_TRICUBETEXTURE_H__
#endif