#pragma once

#ifndef __LADDER_H__
#define __LADDER_H__

#include "MyObject.h"

BEGIN(Client)

// ��� �浹X
class CLadder : public CMyObject
{
private:
	explicit CLadder(LPDIRECT3DDEVICE9 pDevice);
	explicit CLadder(const CLadder& other);
	virtual ~CLadder() = default;

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

public:
	static CLadder* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CCollision_Cube*	m_pCubeCollisionCom = nullptr;

	// ��ٸ�

	// �߷�X, �浹O

	// �浹
	// �÷��̾�� �浹 ��,
	// ~> �׳� �浹 : �ƹ��ϵ� ���Ͼ
	// ~> �̵�Ű ���� ���·� �浹 : ��ٸ� Ž(�� �Ʒ��� �̵�)
};

END

#endif // !__LADDER_H__


