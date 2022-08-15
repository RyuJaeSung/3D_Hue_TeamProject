#pragma once

#ifndef __MOVEBOX_H__
#define __MOVEBOX_H__

#include "MyObject.h"

BEGIN(Client)

// 얘는 충돌X
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
	// CMyObject을(를) 통해 상속됨
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

	//상자 이동하는 함수
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
	// 혼자서 왔다갔다 하는 상자

	// 중력X, 이동O, 충돌O

	// 이동
	// 설정해준 DIR_OPTION에 따라 위<->아래 / 왼쪽<->오른쪽 이동

	// 충돌
	// 플레이어와 충돌 시, 플레이어가 상자 위에 있으면 상자 움직임에 따라 위아래로 움직임
	// 레이저와 충돌 시, 레이저 멈춤
};

END

#endif // !__MOVEBOX_H__


