#pragma once

#ifndef __ROCK_H__
#define __ROCK_H__

#include "MyObject.h"

BEGIN(Client)
//만들어지는 시간값
#define CREATETIME 2.5f

// 얘는 충돌X
class CRock final : public CMyObject
{
	//처음 위치에 따른 방향값
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
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	//돌 굴러갈 방향(바닥의 무게중심을 비교해서 정함)
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

	//같은 위치에 시간값에 따라 그리기 위한 변수
	float m_fCreateTime = 0.f;
	bool m_IsCreate = true;
	bool m_IsDir = true;

	//시작 위치값
	BaseINFO m_tStartInfo;

	bool	 m_bRender = false;

};

END

#endif // !__ROCK_H__


