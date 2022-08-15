#pragma once

#ifndef __LADDER_H__
#define __LADDER_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CLadder : public CMyObject
{
private:
	explicit CLadder(LPDIRECT3DDEVICE9 pDevice);
	explicit CLadder(const CLadder& other);
	virtual ~CLadder() = default;

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

public:
	static CLadder* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CCollision_Cube*	m_pCubeCollisionCom = nullptr;

	// 사다리

	// 중력X, 충돌O

	// 충돌
	// 플레이어와 충돌 시,
	// ~> 그냥 충돌 : 아무일도 안일어남
	// ~> 이동키 누른 상태로 충돌 : 사다리 탐(위 아래로 이동)
};

END

#endif // !__LADDER_H__


