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
	virtual HRESULT Ready_Component_Prototype() = 0; // ���� ���� �� �ʱ�ȭ
	virtual HRESULT Ready_Component(void* pArg) = 0; // ������ ���� �� �ʱ�ȭ	
	virtual HRESULT Render_VIBuffer() = 0;

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; // ������Ÿ�� ����
	virtual void Free() override;

protected:
	// ���ؽ� ����: ����ڰ� �����س��� ���ؽ����� �迭 �������� �����ϰ� �ִ�.
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;	// ���ؽ� ���� �İ�ü ������.	

	DWORD m_dwFVF = 0;
	DWORD m_dwVtxSize = 0; // ���ؽ� ���� ������. (�츮�� �����س��� ����ü ������)
	DWORD m_dwVtxCount = 0; // �� ���ؽ� ����.
	DWORD m_dwTriCount = 0; // �ﰢ�� ����.

	// �ε��� ����: ����ڰ� �����س��� �ε������� �迭 �������� �����ϰ� �ִ�.
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	DWORD		m_dwIndexSize = 0;
	D3DFORMAT	m_IndexFmt; // �ε��� ���� (16��Ʈ¥��? 32��Ʈ¥��?)
};

END

#define __VIBUFFER_H__
#endif
