#pragma once
#ifndef __GRAVITY_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGravity : public CComponent
{
public:
	typedef struct GravityInfo
	{
		//중력
		D3DXVECTOR3	vGravity = { 0.f, 0.f, 0.f };
		//중력 가속도
		D3DXVECTOR3 vVelocity = { 0.f, 0.f, 0.f };
	};

private:
	explicit CGravity(LPDIRECT3DDEVICE9 pDevice);
	explicit CGravity(const CGravity& other);
	virtual ~CGravity() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;

public:
	//Get
	const D3DXVECTOR3& Get_Gravity();
	const D3DXVECTOR3& Get_Velocity();
	//Set
	void Set_floatGravity(const float _fGravity);
	void Set_Gravity(const D3DXVECTOR3& _vGravity);


public:
	void Free_Fall(D3DXVECTOR3& _vPosition, float _fDeltaTime);
	void Clear_Velocity();
	void Clear_Gravity();

public:
	static CGravity* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
	
private:
	GravityInfo	m_tGravity;
};

END
#define __GRAVITY_H__
#endif // !__GRAVITY_H__
