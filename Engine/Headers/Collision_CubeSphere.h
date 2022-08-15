#pragma once

#ifndef __COLLISION_CUBESPHERE_H__
#define __COLLISION_CUBESPHERE_H__

#include "Collision.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_CubeSphere final : public CCollision
{
public:
	typedef struct tagCollision_CubeSphereDesc
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		float fRadius = 0.f;
	}COLLISION_CUBESPHERE_DESC;

private:
	explicit CCollision_CubeSphere(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_CubeSphere(const CCollision_CubeSphere& other);
	virtual ~CCollision_CubeSphere() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override; // 원본 생성 시 초기화
	virtual HRESULT Ready_Component(void* pArg) override; // 복제본 생성 시 초기화	

public:
	HRESULT SetUp_SphereDesc(const D3DXVECTOR3& _vCenterPos, const float& _fRadius);
	bool IsCollision_ToDestSphere(D3DXVECTOR3* _pOut, const D3DXVECTOR3& _vCenterPos, const D3DXVECTOR3& _vHalfScale);


public:
	static CCollision_CubeSphere* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component(void* pArg) override;
	virtual void Free() override;


private:
	COLLISION_CUBESPHERE_DESC m_Desc;
};

END

#endif // !__COLLISION_CUBESPHERE_H__
