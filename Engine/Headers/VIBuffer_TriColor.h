#pragma once

#ifndef __VIBUFFER_TRICOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TriColor final : public CVIBuffer
{
public:
	explicit CVIBuffer_TriColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_TriColor(const CVIBuffer_TriColor& other);
	virtual ~CVIBuffer_TriColor() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_TriColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
};

END

#define __VIBUFFER_TRICOLOR_H__
#endif