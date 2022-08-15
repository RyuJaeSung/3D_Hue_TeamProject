#pragma once

#ifndef __FIXBOX_H__
#define __FIXBOX_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CFixBox final : public CMyObject
{
private:
	explicit CFixBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CFixBox(const CFixBox& other);
	virtual ~CFixBox() = default;

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT SetUp_CollisionDesc();

	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer();
	HRESULT Collision_ToPushBox();

public:
	static CFixBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CCollision_Cube*	m_pCubeCollisionCom = nullptr;

};

END

#endif // !__FIXBOX_H__


