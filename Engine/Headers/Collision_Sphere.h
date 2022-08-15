#pragma once

#ifndef __COLLISION_SPHERE_H__
#define __COLLISION_SPHERE_H__

#include "Collision.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Sphere final : public CCollision
{
public:
	typedef struct tagCollision_SphereDesc
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		float fRadius = 0.f;
	}COLLISION_SPHERE_DESC;

private:
	explicit CCollision_Sphere(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_Sphere(const CCollision_Sphere& other);
	virtual ~CCollision_Sphere() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override; // 원본 생성 시 초기화
	virtual HRESULT Ready_Component(void* pArg) override; // 복제본 생성 시 초기화	

public:
	HRESULT SetUp_SphereDesc(const D3DXVECTOR3& _vCenterPos, const float& _fRadius);
	bool IsCollision_ToDestSphere(const D3DXVECTOR3& _vCenterPos, const float& _fRadius);


public:
	static CCollision_Sphere* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component(void* pArg) override; 
	virtual void Free() override;


private:
	COLLISION_SPHERE_DESC m_Desc;
};

END

#endif // !__COLLISION_SPHERE_H__
