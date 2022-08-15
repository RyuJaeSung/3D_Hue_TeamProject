#pragma once

#ifndef __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectTexture final : public CVIBuffer
{
public:
	explicit CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_RectTexture(const CVIBuffer_RectTexture& other);
	virtual ~CVIBuffer_RectTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_RectTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
};

END

#define __VIBUFFER_RECTTEXTURE_H__
#endif