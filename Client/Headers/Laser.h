#pragma once

#ifndef __LASER_H__
#define __LASER_H__

#include "MyObject.h"

BEGIN(Client)

class CLaserGun;
class CLaser : public CMyObject
{
private:
	explicit CLaser(LPDIRECT3DDEVICE9 pDevice);
	explicit CLaser(const CLaser& other);
	virtual ~CLaser() = default;

public:
	// Getter

	// Setter
	HRESULT Set_LaserGun(CLaserGun* _pIn);
	void Set_RevRadian(const D3DXVECTOR3& _vRevRadian) { m_vRevRadian = _vRevRadian; }

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
	HRESULT Collision_ToTile();
	HRESULT Collision_ToObject(const wstring& _wstrObjLayerTag);
	HRESULT Collision_ToPlayer();

	HRESULT Movement(float _fTimeDelta);
	HRESULT FrameMove(float _fTimeDelta, float _fFrameSpeed);

	HRESULT Create_Laser_Effect();
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc);


public:
	static CLaser* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CLaserGun*				m_pLaserGun = nullptr;
	CCollision_Raycast*		m_pRaycast_CollisionCom = nullptr;

	float			m_fMaxLength = 0.f; // 레이저의 최대 길이
	float			m_fLength = 0.f;
	D3DXVECTOR3		m_vRevRadian = { 0.f, 0.f, 0.f }; // 공전 각도

	float			m_fCurTextureIndex = 0.f; // 계속 갱신되는 텍스쳐 인덱스
	float			m_fMaxTextureIndex = 5.f; // 최대 텍스쳐 인덱스

	bool			m_bIsInit = false;
	D3DXVECTOR3		m_vLaserDir = { 0.f, 0.f, 0.f };

	D3DXVECTOR3		m_vLaser_EffectPos = { 0.f, 0.f, 0.f };
};

END

#endif // !__LASER_H__