#pragma once

#ifndef __FRUSTUM_H__
#define __FRUSTOM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CComponent
{
private:
public:
	explicit CFrustum(LPDIRECT3DDEVICE9 pDevice);
	explicit CFrustum(const CFrustum& other);
	virtual ~CFrustum() = default;

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;

public:
	void ConstructFrustum(float _fFar, D3DXMATRIX _matProjection, D3DXMATRIX _matView);

public:
	bool Check_Cube(const D3DXVECTOR3& _vCenterPos, const D3DXVECTOR3& _vHalfScale);

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;

private:
	D3DXPLANE m_planes[6];
};

END

#endif // !__FRUSTUM_H__


