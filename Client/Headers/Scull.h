#pragma once

#ifndef __SCULL_H__
#define __SCULL_H__

#include "MyObject.h"

BEGIN(Client)

//��鸮�� �ð� ���ǰ�
#define SHAKETIME			2.f
#define SCULLGRAVITY		0.1f

// ��� �浹X
class CScull : public CMyObject
{
private:
	explicit CScull(LPDIRECT3DDEVICE9 pDevice);
	explicit CScull(const CScull& other);
	virtual ~CScull() = default;

	enum STATE { STAND, SHAKE, UP, DOWN };

public:
	// CMyObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT Collision_ToPlayer();
	bool Collision_ToTile();
	bool Collision_ToPushBox();
	bool Collision_ToFixBox();

	//�÷��̾�� �ڽ��̶� x,z���� ������ Ȯ��
	HRESULT		CollisionCheck();
	//��鸲
	HRESULT		Shake(float fTimeDelta);
	HRESULT		Up(float fTimeDelta);
	HRESULT		Down(float fTimeDelta);

	//��鸲 ��ǥ �����
	void	Shake_Effect(float _fTimeDelta);
	HRESULT Create_Dust_Effect();

public:
	static CScull* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	D3DXVECTOR3		m_vStartPos;
	STATE			m_eState;
	//��鸮�� �ð� ���� ��
	float			m_fShakeTime;

};

END

#endif // !__SCULL_H__


