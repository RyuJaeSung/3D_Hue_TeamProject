#pragma once

#ifndef __KEY_H__
#define __KEY_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CKey : public CMyObject
{
private:
	explicit CKey(LPDIRECT3DDEVICE9 pDevice);
	explicit CKey(const CKey& other);
	virtual ~CKey() = default;

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer(float _fTimeDelta);
	HRESULT Create_Item_Effect();
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc);

public:
	static CKey* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	float	m_fCreateTime = 0.f;
};

END

#endif // !__KEY_H__


