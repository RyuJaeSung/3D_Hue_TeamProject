#pragma once

#ifndef __BALLOONBOX_H__
#define __BALLOONBOX_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CBalloon;
class CBalloonBox final : public CMyObject
{
private:
	explicit CBalloonBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CBalloonBox(const CBalloonBox& other);
	virtual ~CBalloonBox() = default;

public:
	// Getter
	bool Get_IsFrustum() { return m_bInFrustum; }

	// Setter
	HRESULT Set_Balloon(CBalloon* _pIn);

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT SetUp_BoxPosition();
	HRESULT Collision_ToPlayer(float _fTimeDelta);
	HRESULT Collision_ToTile();
	HRESULT Collision_ToSpringBox();

public:
	static CBalloonBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CBalloon*	m_pBalloon = nullptr;

	bool			m_bCollision = false;
};

END

#endif // !__BALLOONBOX_H__

