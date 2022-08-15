#pragma once

#ifndef __BALLOON_H__
#define __BALLOON_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CBalloonBox;
class CBalloon final : public CMyObject
{
private:
	explicit CBalloon(LPDIRECT3DDEVICE9 pDevice);
	explicit CBalloon(const CBalloon& other);
	virtual ~CBalloon() = default;

public:
	// Getter

	// Setter
	void Set_Pos(D3DXVECTOR3 vPos);
	void Set_BalloonBox_Color(const COLOR_TYPE& _eColorType) { m_eBoxColor = _eColorType; }
	void Set_Move(const bool& _bMove) { m_bMove = _bMove; }

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT Create_BalloonBox();
	HRESULT Collision_ToTile(float _fTimeDelta);
	HRESULT Collision_ToSpringBox(float _fTimeDelta);

public:
	static CBalloon* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	COLOR_TYPE		m_eBoxColor = COLOR_TYPE::COLOR_END;
	CBalloonBox*	m_pBalloonBox = nullptr;

	bool			m_bMove = true;
	bool			m_bCollision = false;
};

END

#endif // !__BALLOON_H__

