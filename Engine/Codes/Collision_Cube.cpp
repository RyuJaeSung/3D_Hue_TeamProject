#include "..\Headers\Collision_Cube.h"

USING(Engine)

CCollision_Cube::CCollision_Cube(LPDIRECT3DDEVICE9 pDevice)
	: CCollision(pDevice)
{
}

CCollision_Cube::CCollision_Cube(const CCollision_Cube & other)
	: CCollision(other)
{
}

HRESULT CCollision_Cube::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollision_Cube::Ready_Component(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_Desc, pArg, sizeof(COLLISION_CUBE_DESC));

	return S_OK;
}

HRESULT CCollision_Cube::SetUp_CubeDesc(const D3DXVECTOR3 & _vCenterPos, const D3DXVECTOR3 & _vHalfScale)
{
	m_Desc.vCenterPos = _vCenterPos;

	m_Desc.vMaxPos = _vCenterPos;
	m_Desc.vMaxPos.x += _vHalfScale.x;
	m_Desc.vMaxPos.y += _vHalfScale.y;
	m_Desc.vMaxPos.z += _vHalfScale.z;

	m_Desc.vMinPos = _vCenterPos;
	m_Desc.vMinPos.x -= _vHalfScale.x;
	m_Desc.vMinPos.y -= _vHalfScale.y;
	m_Desc.vMinPos.z -= _vHalfScale.z;

	return S_OK;
}

bool CCollision_Cube::IsCollision_ToDestCube(D3DXVECTOR3* _pOut, const D3DXVECTOR3 & _vCenterPos, const D3DXVECTOR3 & _vHalfScale)
{
	// 충돌할 대상의 MaxPos, MinPos 초기화
	D3DXVECTOR3 vSrc_MaxPos = {};
	D3DXVECTOR3 vSrc_MinPos = {};

	vSrc_MaxPos = _vCenterPos;
	vSrc_MaxPos.x += _vHalfScale.x;
	vSrc_MaxPos.y += _vHalfScale.y;
	vSrc_MaxPos.z += _vHalfScale.z;

	vSrc_MinPos = _vCenterPos;
	vSrc_MinPos.x -= _vHalfScale.x;
	vSrc_MinPos.y -= _vHalfScale.y;
	vSrc_MinPos.z -= _vHalfScale.z;

	// AABB충돌
	if (m_Desc.vMinPos.x <= vSrc_MaxPos.x && m_Desc.vMaxPos.x >= vSrc_MinPos.x &&
		m_Desc.vMinPos.y <= vSrc_MaxPos.y && m_Desc.vMaxPos.y >= vSrc_MinPos.y &&
		m_Desc.vMinPos.z <= vSrc_MaxPos.z && m_Desc.vMaxPos.z >= vSrc_MinPos.z)
	{

		D3DXVECTOR3 vBounding = { 0.f, 0.f, 0.f };

													
		if (m_Desc.vCenterPos.x < _vCenterPos.x)
			vBounding.x = m_Desc.vMaxPos.x - vSrc_MinPos.x;
		else
			vBounding.x = vSrc_MaxPos.x - m_Desc.vMinPos.x;

		if (m_Desc.vCenterPos.y < _vCenterPos.y)
			vBounding.y = m_Desc.vMaxPos.y - vSrc_MinPos.y;
		else
			vBounding.y = vSrc_MaxPos.y - m_Desc.vMinPos.y;

		if (m_Desc.vCenterPos.z < _vCenterPos.z)
			vBounding.z = m_Desc.vMaxPos.z - vSrc_MinPos.z;
		else
			vBounding.z = vSrc_MaxPos.z - m_Desc.vMinPos.z;


		if (vBounding.x > vBounding.y)
		{
			if (vBounding.y > vBounding.z)
			{
				if (m_Desc.vCenterPos.z < _vCenterPos.z)
					vBounding =  D3DXVECTOR3{ 0.f, 0.f, -vBounding.z };
				else
					vBounding = D3DXVECTOR3{ 0.f, 0.f, vBounding.z };
			}
			else
			{
				if (m_Desc.vCenterPos.y < _vCenterPos.y)
					vBounding = D3DXVECTOR3{ 0.f, -vBounding.y, 0.f };
				else
					vBounding = D3DXVECTOR3{ 0.f, vBounding.y, 0.f };
			}
		}
		else
		{
			if (vBounding.x > vBounding.z)
			{
				if (m_Desc.vCenterPos.z < _vCenterPos.z)
					vBounding = D3DXVECTOR3{ 0.f, 0.f, -vBounding.z };
				else
					vBounding = D3DXVECTOR3{ 0.f, 0.f, vBounding.z };
			}
			else
			{
				if (m_Desc.vCenterPos.x < _vCenterPos.x)
					vBounding = D3DXVECTOR3{ -vBounding.x, 0.f, 0.f };
				else
					vBounding = D3DXVECTOR3{ vBounding.x, 0.f, 0.f };
			}
		}

		*_pOut = vBounding;

		return true;
	}

	return false;
}

CCollision_Cube * CCollision_Cube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_Cube* pInstance = new CCollision_Cube(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCollision_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision_Cube::Clone_Component(void * pArg)
{
	CCollision_Cube* pInstance = new CCollision_Cube(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CCollision_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Cube::Free()
{
	CCollision::Free();
}
