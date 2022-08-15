#pragma once

#ifndef __SPRINGBOX_H__
#define __SPRINGBOX_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
class CSpringBox final: public CMyObject
{
private:
	explicit CSpringBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpringBox(const CSpringBox& other);
	virtual ~CSpringBox() = default;

public:
	// CMyObject을(를) 통해 상속됨
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

	bool			m_bIsTransScale = false;			// 지금 크기를 바꾸고 있는가?
	D3DXVECTOR3		m_vScale = {1.f, 1.f, 1.f};			// 크기 변경을 위한 변수
	float			m_fLimitTime = 0.f;
	float			m_fScaleTime = 0.f;

	// 닿으면 닿은 대상이 튀어오르는 상자

	// 중력X, 이동X, 충돌O, 탄성O

	// 충돌, 탄성
	// 플레이어와 충돌 시, 위로 충돌하면 플레이어가 점프함 + 상자 색상 바뀜(다음 색상으로)
	//	~> 상자가 Y축으로 작아졌다 커지면 더 역동적일 듯
		
	// 레이저와 충돌 시, 레이저 멈춤
};

END

#endif // !__SPRINGBOX_H__


