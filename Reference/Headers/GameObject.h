#pragma once

#ifndef __GAMEOBJECT_H__

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject(const CGameObject& other);
	virtual ~CGameObject() = default;

public:
	// Getter
	CComponent* Find_Component(const wstring& ComponentTag);
	bool Is_Alived() const;
	// Setter
	void Dead_Ailved();

public:
	virtual HRESULT Ready_GameObject_Prototype() = 0; // 원본 생성 시 초기화
	virtual HRESULT Ready_GameObject(void* pArg) = 0; // 복제본 생성 시 초기화
	virtual int Update_GameObject(float fTimeDelta) = 0;
	virtual int LateUpdate_GameObject(float fTimeDelta) = 0;
	virtual HRESULT Render_GameObject() = 0;

protected:
	HRESULT Add_Component(int iSceneID, const wstring& PrototypeTag, const wstring& ComponentTag, CComponent** ppComponent, void* pArg = nullptr);

public:	
	// Clone
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

	typedef unordered_map<wstring, CComponent*>	COMPONENTS;
	COMPONENTS m_Components;

	bool m_IsAlived = true;
};

END

#define __GAMEOBJECT_H__
#endif