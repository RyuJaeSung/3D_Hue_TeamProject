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
	virtual HRESULT Ready_Component_Prototype() = 0; // 원본 생성 시 초기화
	virtual HRESULT Ready_Component(void* pArg) = 0; // 복제본 생성 시 초기화	
	virtual HRESULT Render_VIBuffer() = 0;

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; // 프로토타입 패턴
	virtual void Free() override;

protected:
	// 버텍스 버퍼: 사용자가 정의해놓은 버텍스들을 배열 형식으로 관리하고 있다.
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;	// 버텍스 버퍼 컴객체 포인터.	

	DWORD m_dwFVF = 0;
	DWORD m_dwVtxSize = 0; // 버텍스 개당 사이즈. (우리가 정의해놓은 구조체 사이즈)
	DWORD m_dwVtxCount = 0; // 총 버텍스 개수.
	DWORD m_dwTriCount = 0; // 삼각형 개수.

	// 인덱스 버퍼: 사용자가 정의해놓은 인덱스들을 배열 형식으로 관리하고 있다.
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	DWORD		m_dwIndexSize = 0;
	D3DFORMAT	m_IndexFmt; // 인덱스 포맷 (16비트짜리? 32비트짜리?)
};

END

#define __VIBUFFER_H__
#endif
