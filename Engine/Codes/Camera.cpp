#include "..\Headers\Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CCamera::CCamera(const CCamera & other)
	: CGameObject(other)
{
}

const CCamera::CAMERA_DESC& CCamera::Get_CameraDesc() const
{
	return m_CameraDesc;
}

HRESULT CCamera::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERA_DESC));

	return S_OK;
}

int CCamera::Update_GameObject(float fTimeDelta)
{
	return 0;
}

int CCamera::LateUpdate_GameObject(float fTimeDelta)
{
	D3DXMatrixLookAtLH(
		&m_CameraDesc.matView,
		&m_CameraDesc.vEye, 
		&m_CameraDesc.vAt, 
		&m_CameraDesc.vUp );	

	D3DXMatrixPerspectiveFovLH(
		&m_CameraDesc.matProj,
		m_CameraDesc.fFovY, 
		m_CameraDesc.fAspect, 
		m_CameraDesc.fNear, 
		m_CameraDesc.fFar);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj);

	return 0;
}

HRESULT CCamera::Render_GameObject()
{
	return S_OK;
}

void CCamera::Free()
{
	CGameObject::Free();
}
