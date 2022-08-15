#pragma once

#ifndef __LASERGUN_H__
#define __LAGERGUN_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CLaserGun : public CMyObject
{
private:
	explicit CLaserGun(LPDIRECT3DDEVICE9 pDevice);
	explicit CLaserGun(const CLaserGun& other);
	virtual ~CLaserGun() = default;

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT Create_Laser();

public:
	static CLaserGun* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool	m_bIsinit = false;
};

END

#endif // !__LASERGUN_H__


