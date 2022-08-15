#pragma once

#ifndef __MOVEBOX_H__
#define __MOVEBOX_H__

#include "MyObject.h"

BEGIN(Client)

// ��� �浹X
class CMoveBox final : public CMyObject
{
public:
	enum DIR_OPTION { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE, DIR_END };

private:
	explicit CMoveBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CMoveBox(const CMoveBox& other);
	virtual ~CMoveBox() = default;

public:
	// Getter

	// Setter
	void Set_BoxDir(const DIR_OPTION& _eDir) { m_eDirOption = _eDir; }
	void Set_Move_Distance(const float& _fDis) { m_fDis = _fDis; }
	void Set_StartPos(const D3DXVECTOR3& _vStartPos) { m_vStartPos = _vStartPos; }

public:
	// CMyObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer(float _fTimeDelta);
	HRESULT	Collision_ToTile();

	//���� �̵��ϴ� �Լ�
private:
	void Up(float fTimeDelta);
	void Down(float fTimeDelta);
	void Right(float fTimeDelta);
	void Left(float fTimeDelta);


public:
	static CMoveBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;


private:
	DIR_OPTION		m_eDirOption = DIR_END;

	float			m_fDis = 0.f;
	D3DXVECTOR3		m_vSpeed = { 0.f, 0.f, 0.f };
	D3DXVECTOR3		m_vStartPos = { 0.f, 0.f, 0.f };
	// ȥ�ڼ� �Դٰ��� �ϴ� ����

	// �߷�X, �̵�O, �浹O

	// �̵�
	// �������� DIR_OPTION�� ���� ��<->�Ʒ� / ����<->������ �̵�

	// �浹
	// �÷��̾�� �浹 ��, �÷��̾ ���� ���� ������ ���� �����ӿ� ���� ���Ʒ��� ������
	// �������� �浹 ��, ������ ����
};

END

#endif // !__MOVEBOX_H__


