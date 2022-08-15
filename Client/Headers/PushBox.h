#pragma once

#ifndef __PUSHBOX_H__
#define __PUSHBOX_H__

#include "MyObject.h"

BEGIN(Client)

// ��� �浹X
class CPushBox : public CMyObject
{
private:
	explicit CPushBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CPushBox(const CPushBox& other);
	virtual ~CPushBox() = default;

public:
	// Getter

	// Setter
	void Set_Gravity(const bool& _bGravity) { m_bGravity = _bGravity; }


public:
	// CMyObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT SetUp_CollisionDesc();

private:
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer();
	HRESULT Collision_ToTile(float _fTimeDelta);
	HRESULT Collision_ToPushBox();

public:
	static CPushBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	// Pushbox�� ��ȣ�ۿ� �ϴ� ��� �� ������Ʈ�� �����ؼ� ó��
	CCollision_Cube*	m_pCubeCollisionCom = nullptr;
	bool				m_bGravity = false;
};

END

#endif // !__PUSHBOX_H__


