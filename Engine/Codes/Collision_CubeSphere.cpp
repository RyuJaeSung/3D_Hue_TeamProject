#include "..\Headers\Collision_CubeSphere.h"


USING(Engine)

CCollision_CubeSphere::CCollision_CubeSphere(LPDIRECT3DDEVICE9 pDevice)
	: CCollision(pDevice)
{
}

CCollision_CubeSphere::CCollision_CubeSphere(const CCollision_CubeSphere & other)
	: CCollision(other)
{
}

HRESULT CCollision_CubeSphere::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollision_CubeSphere::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CCollision_CubeSphere::SetUp_SphereDesc(const D3DXVECTOR3 & _vCenterPos, const float & _fRadius)
{
	m_Desc.vPos = _vCenterPos;
	m_Desc.fRadius = _fRadius;

	return S_OK;
}

bool CCollision_CubeSphere::IsCollision_ToDestSphere(D3DXVECTOR3* _pOut, const D3DXVECTOR3 & _vCenterPos, const D3DXVECTOR3 & _vHalfScale)
{
	D3DXVECTOR3 vBoxPoint = {};

	if (m_Desc.vPos.x < _vCenterPos.x - _vHalfScale.x)
		vBoxPoint.x = _vCenterPos.x - _vHalfScale.x;
	else if (m_Desc.vPos.x > _vCenterPos.x + _vHalfScale.x)
		vBoxPoint.x = _vCenterPos.x + _vHalfScale.x;
	else
		vBoxPoint.x = m_Desc.vPos.x;

	if (m_Desc.vPos.y < _vCenterPos.y - _vHalfScale.y)
		vBoxPoint.y = _vCenterPos.y - _vHalfScale.y;
	else if (m_Desc.vPos.y > _vCenterPos.y + _vHalfScale.y)
		vBoxPoint.y = _vCenterPos.y + _vHalfScale.y ;
	else
		vBoxPoint.y = m_Desc.vPos.y;

	if (m_Desc.vPos.z < _vCenterPos.z - _vHalfScale.z)
		vBoxPoint.z = _vCenterPos.z - _vHalfScale.z;
	else if (m_Desc.vPos.z > _vCenterPos.z + _vHalfScale.z)
		vBoxPoint.z = _vCenterPos.z + _vHalfScale.z;
	else
		vBoxPoint.z = m_Desc.vPos.z;

	D3DXVECTOR3 vDist = m_Desc.vPos - vBoxPoint; 
	float fColDistance = D3DXVec3Length(&vDist) - m_Desc.fRadius;

	if (fColDistance <= 0.f) 
	{
		D3DXVECTOR3 vBounding = { 0.f, 0.f, 0.f };

		if (vDist.x != 0.f)
		{
			if (m_Desc.vPos.x < _vCenterPos.x)
				vBounding.x = -fColDistance;
			if (m_Desc.vPos.x > _vCenterPos.x)
				vBounding.x = fColDistance;
		}
		else if (vDist.y != 0.f)
		{
			if (m_Desc.vPos.y < _vCenterPos.y)
				vBounding.y = -fColDistance;
			if (m_Desc.vPos.y > _vCenterPos.y)
				vBounding.y = fColDistance;
		}
		else if (vDist.z != 0.f)
		{
			if (m_Desc.vPos.z < _vCenterPos.z)
				vBounding.z = -fColDistance;
			if (m_Desc.vPos.z > _vCenterPos.z)
				vBounding.z = fColDistance;
		}

		*_pOut = vBounding;

		return true;
	}

	return false;
}

CCollision_CubeSphere * CCollision_CubeSphere::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_CubeSphere* pInstance = new CCollision_CubeSphere(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCollision_CubeSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision_CubeSphere::Clone_Component(void * pArg)
{
	CCollision_CubeSphere* pInstance = new CCollision_CubeSphere(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CCollision_CubeSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_CubeSphere::Free()
{
	CCollision::Free();
}
