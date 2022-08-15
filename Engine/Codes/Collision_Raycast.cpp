#include "..\Headers\Collision_Raycast.h"

USING(Engine)

CCollision_Raycast::CCollision_Raycast(LPDIRECT3DDEVICE9 pDevice)
	: CCollision(pDevice)
{
}

CCollision_Raycast::CCollision_Raycast(const CCollision_Raycast & other)
	: CCollision(other)
{
}

HRESULT CCollision_Raycast::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollision_Raycast::Ready_Component(void * pArg)
{
	return S_OK;
}

bool CCollision_Raycast::IsCollsion_ToCube(float * _pOutDistance, const D3DXVECTOR3 & _vRayPos, const D3DXVECTOR3 & _vRayDir
											, const D3DXVECTOR3 & _vSrcPos, const D3DXVECTOR3 & _vSrcHalfSize)
{
	if (nullptr == _pOutDistance)
		return false;

	D3DXVECTOR3 vCube_VertexPos[8] = {};
	// 큐브의 정점들 > VIBuffer_CubeTexture 순서
	vCube_VertexPos[0] = D3DXVECTOR3(_vSrcPos.x - _vSrcHalfSize.x, _vSrcPos.y + _vSrcHalfSize.y, _vSrcPos.z - _vSrcHalfSize.z);
	vCube_VertexPos[1] = D3DXVECTOR3(_vSrcPos.x + _vSrcHalfSize.x, _vSrcPos.y + _vSrcHalfSize.y, _vSrcPos.z - _vSrcHalfSize.z);
	vCube_VertexPos[2] = D3DXVECTOR3(_vSrcPos.x + _vSrcHalfSize.x, _vSrcPos.y - _vSrcHalfSize.y, _vSrcPos.z - _vSrcHalfSize.z);
	vCube_VertexPos[3] = D3DXVECTOR3(_vSrcPos.x - _vSrcHalfSize.x, _vSrcPos.y - _vSrcHalfSize.y, _vSrcPos.z - _vSrcHalfSize.z);
	vCube_VertexPos[4] = D3DXVECTOR3(_vSrcPos.x - _vSrcHalfSize.x, _vSrcPos.y + _vSrcHalfSize.y, _vSrcPos.z + _vSrcHalfSize.z);
	vCube_VertexPos[5] = D3DXVECTOR3(_vSrcPos.x + _vSrcHalfSize.x, _vSrcPos.y + _vSrcHalfSize.y, _vSrcPos.z + _vSrcHalfSize.z);
	vCube_VertexPos[6] = D3DXVECTOR3(_vSrcPos.x + _vSrcHalfSize.x, _vSrcPos.y - _vSrcHalfSize.y, _vSrcPos.z + _vSrcHalfSize.z);
	vCube_VertexPos[7] = D3DXVECTOR3(_vSrcPos.x - _vSrcHalfSize.x, _vSrcPos.y - _vSrcHalfSize.y, _vSrcPos.z + _vSrcHalfSize.z);
	
	float fU = 0.f, fV = 0.f, fDist = 0.f;
	bool bCollsion = false;
	float fShortest_Dist = 0.f; // 최단거리

#pragma region CUBE_RIGHT
	if (D3DXIntersectTri(
		&vCube_VertexPos[1],
		&vCube_VertexPos[5],
		&vCube_VertexPos[6],
		&_vRayPos,
		&_vRayDir,
		&fU,
		&fV, 
		&fDist /))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist)
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[1],
		&vCube_VertexPos[6],
		&vCube_VertexPos[2],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}
#pragma endregion

#pragma region CUBE_LEFT
	if (D3DXIntersectTri(
		&vCube_VertexPos[4],
		&vCube_VertexPos[0],
		&vCube_VertexPos[3],
		&_vRayPos,
		&_vRayDir,
		&fU,
		&fV, 
		&fDist))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[4],
		&vCube_VertexPos[3],
		&vCube_VertexPos[7],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}
#pragma endregion

#pragma region CUBE_UP
	if (D3DXIntersectTri(
		&vCube_VertexPos[4],
		&vCube_VertexPos[5],
		&vCube_VertexPos[1],
		&_vRayPos,
		&_vRayDir,
		&fU,
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[4],
		&vCube_VertexPos[1],
		&vCube_VertexPos[0],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}
#pragma endregion

#pragma region CUBE_DOWN
	if (D3DXIntersectTri(
		&vCube_VertexPos[6],
		&vCube_VertexPos[7],
		&vCube_VertexPos[3],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV,
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[6],
		&vCube_VertexPos[3],
		&vCube_VertexPos[2],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}
#pragma endregion

#pragma region CUBE_FRONT
	if (D3DXIntersectTri(
		&vCube_VertexPos[0],
		&vCube_VertexPos[1],
		&vCube_VertexPos[2],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[0],
		&vCube_VertexPos[2],
		&vCube_VertexPos[3],
		&_vRayPos,
		&_vRayDir,
		&fU,
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist)
		{
			bCollsion = true;
			fShortest_Dist = fDist;
		}
	}
#pragma endregion

#pragma region CUBE_BACK
	if (D3DXIntersectTri(
		&vCube_VertexPos[5],
		&vCube_VertexPos[4],
		&vCube_VertexPos[7],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist)
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}

	if (D3DXIntersectTri(
		&vCube_VertexPos[5],
		&vCube_VertexPos[7],
		&vCube_VertexPos[6],
		&_vRayPos,
		&_vRayDir,
		&fU, 
		&fV, 
		&fDist ))
	{
		if (false == bCollsion 
			|| fDist < fShortest_Dist) 
		{
			bCollsion = true;
			fShortest_Dist = fDist; 
		}
	}
#pragma endregion

	*_pOutDistance = fShortest_Dist;

	return bCollsion;
}

CCollision_Raycast * CCollision_Raycast::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_Raycast* pInstance = new CCollision_Raycast(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCollision_Raycast");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision_Raycast::Clone_Component(void * pArg)
{
	CCollision_Raycast* pInstance = new CCollision_Raycast(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CCollision_Raycast");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Raycast::Free()
{
	CCollision::Free();
}
