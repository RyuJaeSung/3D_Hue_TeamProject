#pragma once

#ifndef __ROCK_H__
#define __ROCK_H__

#include "MyObject.h"

BEGIN(Client)
//��������� �ð���
#define CREATETIME 2.5f

// ��� �浹X
class CRock final : public CMyObject
{
	//ó�� ��ġ�� ���� ���Ⱚ
	enum DIR { LEFT, RIGHT, DIR_END };

	typedef struct BaseInfo
	{
		D3DXVECTOR3	vStartPos = { 0.f, 0.f, 0.f };
		COLOR_TYPE	eColor = COLOR_END;
	}BaseINFO;

private:
	enum ROCK_TPYE { ROCK_ALIVE, ROCK_DEAD, ROCK_END};

private:
	explicit CRock(LPDIRECT3DDEVICE9 pDevice);
	explicit CRock(const CRock& other);
	virtual ~CRock() = default;

public:
	void Set_StartPos(const D3DXVECTOR3& _vPos);
	void Set_StartColor(const COLOR_TYPE& _eColor);

public:
	// CMyObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	//�� ������ ����(�ٴ��� �����߽��� ���ؼ� ����)
	void	MoveRock(float fTimeDelta);
	virtual HRESULT Collision(float fTimeDelta) override;
	HRESULT Collision_ToPlayer();
	HRESULT Collision_ToTile();
	HRESULT Create_Dust_Effect();

public:
	static CRock* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CTexture* m_pTextureRock[ROCK_END];
	float	m_fDeadCount = 0.f;
	bool	m_IsDead = false;
	DIR	m_eRockDir = DIR_END;

	//���� ��ġ�� �ð����� ���� �׸��� ���� ����
	float m_fCreateTime = 0.f;
	bool m_IsCreate = true;
	bool m_IsDir = true;

	//���� ��ġ��
	BaseINFO m_tStartInfo;

	bool	 m_bRender = false;

};

END

#endif // !__ROCK_H__


