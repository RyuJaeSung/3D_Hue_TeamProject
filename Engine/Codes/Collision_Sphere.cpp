#include "..\Headers\Collision_Sphere.h"


USING(Engine)

CCollision_Sphere::CCollision_Sphere(LPDIRECT3DDEVICE9 pDevice)
	: CCollision(pDevice)
{
}

CCollision_Sphere::CCollision_Sphere(const CCollision_Sphere & other)
	: CCollision(other)
{
}

HRESULT CCollision_Sphere::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollision_Sphere::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CCollision_Sphere::SetUp_SphereDesc(const D3DXVECTOR3 & _vCenterPos, const float & _fRadius)
{
	m_Desc.vPos = _vCenterPos;
	m_Desc.fRadius = _fRadius;

	return S_OK;
}

bool CCollision_Sphere::IsCollision_ToDestSphere(const D3DXVECTOR3 & _vCenterPos, const float & _fRadius)
{
	float fRadiusSum = m_Desc.fRadius + _fRadius;

	if ((abs((m_Desc.vPos.x) - (_vCenterPos.x))) > fRadiusSum) return false;
	if ((abs((m_Desc.vPos.y) - (_vCenterPos.y))) > fRadiusSum) return false;
	if ((abs((m_Desc.vPos.z) - (_vCenterPos.z))) > fRadiusSum) return false;

	return true;
}

CCollision_Sphere * CCollision_Sphere::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_Sphere* pInstance = new CCollision_Sphere(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCollision_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision_Sphere::Clone_Component(void * pArg)
{
	CCollision_Sphere* pInstance = new CCollision_Sphere(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CCollision_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Sphere::Free()
{
	CCollision::Free();
}
