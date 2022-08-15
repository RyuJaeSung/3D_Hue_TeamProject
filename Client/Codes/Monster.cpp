#include "stdafx.h"
#include "..\Headers\Monster.h"

USING(Client)

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CMonster::CMonster(const CMonster & other)
	: CGameObject(other)
{
}

HRESULT CMonster::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CMonster::Update_GameObject(float fTimeDelta)
{
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	if (FAILED(Collision_ToTerrain()))
		return 0;

	bool CollisionCom = true;
	bool m_bCollision = true;
	m_pCubeCollisionCom->Update_Collision_Cube();

	for (auto& bCol : m_pSphereCollisionCom->Get_IsSphereCol(COLLISION_GROUP::COLLISION_MONSTER))
	{
		if (bCol)
		{
			// 충돌
			CollisionCom = false;
			m_bCollision = false;

			list<CGameObject*> pObj = m_pSphereCollisionCom->Get_AABBObjList(COLLISION_GROUP::COLLISION_MONSTER);

			auto& iterBegin = pObj.begin();
			auto& iterEnd = pObj.end();
			// 충돌 처리
			list<D3DXVECTOR3> m_listColPos = m_pSphereCollisionCom->Get_SphereBoundingList(COLLISION_GROUP::COLLISION_MONSTER);
			for (auto& vecCol : m_listColPos)
			{
				if (vecCol.y != 0.f && iterBegin != iterEnd)
					m_pYBox = (*iterBegin);

				m_pTransformCom->Go_Posion(vecCol);
				m_vPower = vecCol;
				if (iterBegin != iterEnd)
					iterBegin++;
			}
			m_pTransformCom->ClearVelocity();
		}
	}
	for (auto& bCol : m_pSphereCollisionCom->Get_IsSphereCol(COLLISION_GROUP::COLLISION_PLAYER))
	{
		if (bCol)
		{
			// 충돌됨

			// 충돌 처리
			list<D3DXVECTOR3> m_listColPos = m_pSphereCollisionCom->Get_SphereBoundingList(COLLISION_GROUP::COLLISION_PLAYER);
			for (auto& vecCol : m_listColPos)
			{
				m_pTransformCom->Go_Posion(vecCol);

				if (m_pYBox)
				{
					CTransform* m_pYTransform = (CTransform*)m_pYBox->Find_Component(L"Com_Transform");
					m_pYTransform->Go_Posion(vecCol);
				}
			}
		}
	}


	if (m_bCollision)
	{
		if (m_pYBox)
		{
			CTransform* m_pYTransform = (CTransform*)m_pYBox->Find_Component(L"Com_Transform");
			m_pYTransform->Go_Posion(m_vPower);
		}
		m_pYBox = nullptr;
	}


	if (CollisionCom)
	{
		D3DXVECTOR3 vPos;
		m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);
		if (vPos.y >= 0.f)
			m_pTransformCom->Update_Integrate(fTimeDelta);
		else
			m_pTransformCom->ClearVelocity();
	}

	return 0;
}

int CMonster::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_NONALPHA, this)))
		return 0;

	return 0;
}

HRESULT CMonster::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	if (FAILED(m_pTextureCom->Set_Texture(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Add_Component()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Monster",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	//For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vRotate = { 0.f, 0.f, 0.f };
	TransformInfo.vPosition = { float(rand() % 10), 0.f, 5.f };
	TransformInfo.fSpeedPerSec = 5.f;
	TransformInfo.fRadianPerSec = D3DXToRadian(90.f);
	TransformInfo.fDamping = 5.f;
	TransformInfo.fMass = 2.f;


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
	//중력 초기화
	m_pTransformCom->Set_Graity(5.f);
#pragma endregion

#pragma region Com_Cube_Collision
	// For.Com_Cube_Collision
	CCollision_Cube::COLLISION_CUBE_DESC CubeCollisionDesc;
	if (FAILED(m_pVIBufferCom->Get_Vertices((void**)&CubeCollisionDesc.pVtx)))
		return E_FAIL;
	CubeCollisionDesc.dwNumVertices = m_pVIBufferCom->Get_VtxCount();
	CubeCollisionDesc.dwStride = m_pVIBufferCom->Get_VtxSize();
	CubeCollisionDesc.matWorld = &m_pTransformCom->Get_TransformDesc().matWorld;
	CubeCollisionDesc.pPos = &m_pTransformCom->Get_TransformDesc().vPosition;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Cube",
		L"Com_Collision_Cube", (CComponent**)&m_pCubeCollisionCom, &CubeCollisionDesc)))
		return E_FAIL;

	// 충돌 컴포넌트를 매니저에 담는다.
	pManagement->Add_CubeCollisionList(COLLISION_GROUP::COLLISION_MONSTER, m_pCubeCollisionCom);
#pragma endregion

#pragma region Com_Sphere_Collision
	// For.Com_Sphere_Collision
	CCollision_Sphere::COLLISION_SPHERE_DESC SphereCollisionDesc;
	SphereCollisionDesc.fRadius = m_pTransformCom->Get_TransformDesc().vScale.x * 0.5f;				// 충돌 구의 반지름 길이. 상황에 맞춰서 필요한 값을 대입해 사용하면 됩니다.
	SphereCollisionDesc.pPos = &m_pTransformCom->Get_TransformDesc().vPosition;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Sphere",
		L"Com_Collision_Sphere", (CComponent**)&m_pSphereCollisionCom, &SphereCollisionDesc)))
		return E_FAIL;

	// 충돌 컴포넌트를 매니저에 담는다.
	pManagement->Add_SphereCollisionList(COLLISION_GROUP::COLLISION_MONSTER, m_pSphereCollisionCom);
#pragma endregion

#pragma region Com_Collision
	// For.Com_Collision
	CCollision_Terrain::COLLISION_TERRAIN_DESC CollisionDesc;
	CollisionDesc.dwVertexCntX = 129;
	CollisionDesc.dwVertexCntZ = 129;
	CollisionDesc.fVertexItv = 1.f;
	CollisionDesc.pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component_Pointer(SCENE_STAGE, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == CollisionDesc.pTerrainBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Terrain",
		L"Com_Collision", (CComponent**)&m_pCollisionCom, &CollisionDesc)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}
HRESULT CMonster::Collision_ToTerrain()
{
	if (nullptr == m_pTransformCom ||
		nullptr == m_pCollisionCom)
		return E_FAIL;

	D3DXVECTOR3 vPos = m_pTransformCom->Get_TransformDesc().vPosition;

	if (true == m_pCollisionCom->IsCollision_ToTerrain(&vPos))
	{
		m_pTransformCom->Set_Position(vPos);
	}

	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster* pInstance = new CMonster(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone_GameObject(void * pArg)
{
	CMonster* pInstance = new CMonster(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	//
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pCubeCollisionCom);
	Safe_Release(m_pSphereCollisionCom);
	//
	CGameObject::Free();
}
