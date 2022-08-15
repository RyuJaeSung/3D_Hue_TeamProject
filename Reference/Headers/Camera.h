#pragma once

#ifndef __CAMERA_H__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		tagCameraDesc()
		{
			ZeroMemory(&vEye, sizeof(D3DXVECTOR3));
			ZeroMemory(&vAt, sizeof(D3DXVECTOR3));
			ZeroMemory(&vUp, sizeof(D3DXVECTOR3));

			D3DXMatrixIdentity(&matView);
			D3DXMatrixIdentity(&matProj);
		}

		D3DXVECTOR3 vEye, vAt, vUp; // View
		float fFovY = 0.f, fAspect = 0.f, fNear = 0.f, fFar = 0.f; // Projection
		D3DXMATRIX matView, matProj;
	}CAMERA_DESC;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& other);
	virtual ~CCamera() = default;

public:
	const CAMERA_DESC& Get_CameraDesc() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	virtual void Free() override;

protected:
	CAMERA_DESC	m_CameraDesc;
};

END

#define __CAMERA_H__
#endif