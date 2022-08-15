#pragma once

#ifndef __COLLISION_CUBE_H__
#define __COLLISION_CUBE_H__

#include "Collision.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Cube final : public CCollision
{
public:
	typedef struct tagCollision_CubeDesc
	{
		D3DXVECTOR3 vCenterPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vMinPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vMaxPos = { 0.f, 0.f, 0.f };
	}COLLISION_CUBE_DESC;

private:
	explicit CCollision_Cube(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_Cube(const CCollision_Cube& other);
	virtual ~CCollision_Cube() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override; // 원본 생성 시 초기화
	virtual HRESULT Ready_Component(void* pArg) override; // 복제본 생성 시 초기화	

public:
	HRESULT SetUp_CubeDesc(const D3DXVECTOR3& _vCenterPos, const D3DXVECTOR3& _vHalfScale);
	bool IsCollision_ToDestCube(D3DXVECTOR3* _pOut, const D3DXVECTOR3& _vCenterPos, const D3DXVECTOR3& _vHalfScale);


public:
	static CCollision_Cube* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component(void* pArg) override;
	virtual void Free() override;


private:
	COLLISION_CUBE_DESC m_Desc;
};

END

#endif // !__COLLISION_CUBE_H__




