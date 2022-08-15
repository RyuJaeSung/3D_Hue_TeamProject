#pragma once

#ifndef __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& other);
	virtual ~CVIBuffer() = default;

public:
	// Getter
	HRESULT Get_Vertices(void** pOut);
	DWORD Get_VtxSize() { return m_dwVtxSize; }
	DWORD Get_VtxCount() { return m_dwVtxCount; }

	// Setter
	HRESULT Set_Vertices(const void* pIn);

public:
	virtual HRESULT Ready_Component_Prototype() = 0;
	virtual HRESULT Ready_Component(void* pArg) = 0;
	virtual HRESULT Render_VIBuffer() = 0;

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; 
	virtual void Free() override;

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;

	DWORD m_dwFVF = 0;
	DWORD m_dwVtxSize = 0; 
	DWORD m_dwVtxCount = 0;
	DWORD m_dwTriCount = 0; 

	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	DWORD		m_dwIndexSize = 0;
	D3DFORMAT	m_IndexFmt;
};

END

#define __VIBUFFER_H__
#endif
