#include "stdafx.h"
#include "..\Headers\TriCubeTile.h"

USING(Client)

CTriCubeTile::CTriCubeTile(LPDIRECT3DDEVICE9 pDevice)
	: CTile(pDevice)
{
}

CTriCubeTile::CTriCubeTile(const CTriCubeTile & other)
	: CTile(other)
{
}

HRESULT CTriCubeTile::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTriCubeTile::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CTriCubeTile::Update_GameObject(float fTimeDelta)
{
	return CTile::Update_GameObject(fTimeDelta);
}

int CTriCubeTile::LateUpdate_GameObject(float fTimeDelta)
{
	return CTile::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CTriCubeTile::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTriCubeTile::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_TriCubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	//For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 5.f, 1.f, 0.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Stairs",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

//#pragma region Com_Cube_Collision
//	// For.Com_Cube_Collision
//	CCollision_Cube::COLLISION_CUBE_DESC CubeCollisionDesc;
//	if (FAILED(m_pVIBufferCom->Get_Vertices((void**)&CubeCollisionDesc.pVtx)))
//		return E_FAIL;
//	CubeCollisionDesc.dwNumVertices = m_pVIBufferCom->Get_VtxCount();
//	CubeCollisionDesc.dwStride = m_pVIBufferCom->Get_VtxSize();
//	CubeCollisionDesc.matWorld = &m_pTransformCom->Get_TransformDesc().matWorld;
//	CubeCollisionDesc.pPos = &m_pTransformCom->Get_TransformDesc().vPosition;
//
//	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Cube",
//		L"Com_Collision_Cube", (CComponent**)&m_pCubeCollisionCom, &CubeCollisionDesc)))
//		return E_FAIL;
//
//	CManagement* pManagement = CManagement::Get_Instance();
//	if (nullptr == pManagement)
//		return E_FAIL;
//
//	// 충돌 컴포넌트를 매니저에 담는다.
//	//pManagement->Add_CubeCollisionList(COLLISION_GROUP::COLLISION_TILE, m_pCubeCollisionCom);
//#pragma endregion

#pragma region Com_Frustum
	//For.Com_Frustum
	// 컬링 컴포넌트 추가
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Frustum",
		L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

CTriCubeTile * CTriCubeTile::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriCubeTile* pInstance = new CTriCubeTile(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CTriCubeTile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTriCubeTile::Clone_GameObject(void * pArg)
{
	CTriCubeTile* pInstance = new CTriCubeTile(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CTriCubeTile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTriCubeTile::Free()
{
	Safe_Release(m_pVIBufferCom);

	CTile::Free();
}
