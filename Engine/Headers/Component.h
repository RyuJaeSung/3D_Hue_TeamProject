#pragma once

#ifndef __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	explicit CComponent(const CComponent& other);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Ready_Component_Prototype() = 0; // ���� ���� �� �ʱ�ȭ
	virtual HRESULT Ready_Component(void* pArg) = 0; // ������ ���� �� �ʱ�ȭ	

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; // ������Ÿ�� ����
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

END

#define __COMPONENT_H__
#endif