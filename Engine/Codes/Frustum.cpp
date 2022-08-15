#include "..\Headers\Frustum.h"

USING(Engine)

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CFrustum::CFrustum(const CFrustum & other)
	: CComponent(other)
{
}

HRESULT CFrustum::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CFrustum::Ready_Component(void * pArg)
{
	return S_OK;
}

void CFrustum::ConstructFrustum(float _fFar, D3DXMATRIX _matProjection, D3DXMATRIX _matView)
{
	
	float zMinimum = -_matProjection._43 / _matProjection._33;
	float r = _fFar / (_fFar - zMinimum);

	
	_matProjection._33 = r;
	_matProjection._43 = -r * zMinimum;

	
	D3DXMATRIX finalMatrix;
	D3DXMatrixMultiply(&finalMatrix, &_matView, &_matProjection);

	
	float x = (float)(finalMatrix._14 + finalMatrix._13);
	float y = (float)(finalMatrix._24 + finalMatrix._23);
	float z = (float)(finalMatrix._34 + finalMatrix._33);
	float w = (float)(finalMatrix._44 + finalMatrix._43);
	m_planes[0] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	
	x = (float)(finalMatrix._14 - finalMatrix._13);
	y = (float)(finalMatrix._24 - finalMatrix._23);
	z = (float)(finalMatrix._34 - finalMatrix._33);
	w = (float)(finalMatrix._44 - finalMatrix._43);
	m_planes[1] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	
	x = (float)(finalMatrix._14 + finalMatrix._11);
	y = (float)(finalMatrix._24 + finalMatrix._21);
	z = (float)(finalMatrix._34 + finalMatrix._31);
	w = (float)(finalMatrix._44 + finalMatrix._41);
	m_planes[2] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	x = (float)(finalMatrix._14 - finalMatrix._11);
	y = (float)(finalMatrix._24 - finalMatrix._21);
	z = (float)(finalMatrix._34 - finalMatrix._31);
	w = (float)(finalMatrix._44 - finalMatrix._41);
	m_planes[3] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	x = (float)(finalMatrix._14 - finalMatrix._12);
	y = (float)(finalMatrix._24 - finalMatrix._22);
	z = (float)(finalMatrix._34 - finalMatrix._32);
	w = (float)(finalMatrix._44 - finalMatrix._42);
	m_planes[4] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	x = (float)(finalMatrix._14 + finalMatrix._12);
	y = (float)(finalMatrix._24 + finalMatrix._22);
	z = (float)(finalMatrix._34 + finalMatrix._32);
	w = (float)(finalMatrix._44 + finalMatrix._42);
	m_planes[5] = D3DXPLANE(x, y, z, w);
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
}

bool CFrustum::Check_Cube(const D3DXVECTOR3 & _vCenterPos, const D3DXVECTOR3& _vHalfScale)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x - _vHalfScale.x), (_vCenterPos.y - _vHalfScale.y), (_vCenterPos.z - _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x + _vHalfScale.x), (_vCenterPos.y - _vHalfScale.y), (_vCenterPos.z - _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x - _vHalfScale.x), (_vCenterPos.y + _vHalfScale.y), (_vCenterPos.z - _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x + _vHalfScale.x), (_vCenterPos.y + _vHalfScale.y), (_vCenterPos.z - _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x - _vHalfScale.x), (_vCenterPos.y - _vHalfScale.y), (_vCenterPos.z + _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x + _vHalfScale.x), (_vCenterPos.y - _vHalfScale.y), (_vCenterPos.z + _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x - _vHalfScale.x), (_vCenterPos.y + _vHalfScale.y), (_vCenterPos.z + _vHalfScale.z))) >= 0.0f)
			continue;

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((_vCenterPos.x + _vHalfScale.x), (_vCenterPos.y + _vHalfScale.y), (_vCenterPos.z + _vHalfScale.z))) >= 0.0f)
			continue;

		return false;
	}

	return true;
}

CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFrustum* pInstance = new CFrustum(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Load CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CFrustum::Clone_Component(void * pArg)
{
	CFrustum* pInstance = new CFrustum(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	CComponent::Free();
}
