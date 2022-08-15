#pragma once

#ifndef __SPRINGBOX_H__
#define __SPRINGBOX_H__

#include "MyObject.h"

BEGIN(Client)

// ��� �浹X
class CSpringBox final: public CMyObject
{
private:
	explicit CSpringBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpringBox(const CSpringBox& other);
	virtual ~CSpringBox() = default;

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
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer(float _fTimeDelta);

public:
	static CSpringBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CCollision_Cube*	m_pCubeCollisionCom = nullptr;

	bool			m_bIsTransScale = false;			// ���� ũ�⸦ �ٲٰ� �ִ°�?
	D3DXVECTOR3		m_vScale = {1.f, 1.f, 1.f};			// ũ�� ������ ���� ����
	float			m_fLimitTime = 0.f;
	float			m_fScaleTime = 0.f;

	// ������ ���� ����� Ƣ������� ����

	// �߷�X, �̵�X, �浹O, ź��O

	// �浹, ź��
	// �÷��̾�� �浹 ��, ���� �浹�ϸ� �÷��̾ ������ + ���� ���� �ٲ�(���� ��������)
	//	~> ���ڰ� Y������ �۾����� Ŀ���� �� �������� ��
		
	// �������� �浹 ��, ������ ����
};

END

#endif // !__SPRINGBOX_H__


