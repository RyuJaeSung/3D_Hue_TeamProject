#include "stdafx.h"
#include "..\Headers\PushBox.h"

#include "Player_Observer.h"
#include "Tile.h"

USING(Client)

CPushBox::CPushBox(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CPushBox::CPushBox(const CPushBox & other)
	: CMyObject(other)
{
}

HRESULT CPushBox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPushBox::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	// 테스트용 > 나중엔 툴에서 불러오기하면 생성 후 색상 설정
	Set_TextureColor(COLOR_RED);

	return S_OK;
}

int CPushBox::Update_GameObject(float fTimeDelta)
{
	if (m_IsSetting)
	{
		if (nullptr == m_pTransformCom)
			return 0;

		m_pTransformCom->Set_Graity(0.05f);
		D3DXVECTOR3 vScale = m_pTransformCom->Get_TransformDesc().vScale;
		vScale.x *= 0.99f;

		m_pTransformCom->Set_Scale(vScale);

		m_IsSetting = false;
	}

	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
	{
		if (FAILED(Collision(fTimeDelta)))
			return 0;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	return 0;
}

int CPushBox::LateUpdate_GameObject(float fTimeDelta)
{
	m_bGravity = true;

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CPushBox::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CPushBox::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	// 임의로 하나 생성 (레이저 테스트)
	CTransform::TRANSFORM_DESC TransformInfo;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_PushBox",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Collision_Cube
	//For.Com_Collision_Cube
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Cube",
		L"Com_Collision_Cube", (CComponent**)&m_pCubeCollisionCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Frustum
	//For.Com_Frustum
	// 컬링 컴포넌트 추가
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Frustum",
		L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CPushBox::SetUp_CollisionDesc()
{
	if (nullptr == m_pCubeCollisionCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;
	if (0 > vHalfScale.x)
		vHalfScale.x = -vHalfScale.x;

	m_pCubeCollisionCom->SetUp_CubeDesc(vPos, vHalfScale);

	return S_OK;
}

HRESULT CPushBox::Collision(float _fTimeDelta)
{
	if (FAILED(Collision_ToPlayer()))
		return E_FAIL;

	if (FAILED(Collision_ToTile(_fTimeDelta)))
		return E_FAIL;

	if (FAILED(Collision_ToPushBox()))
		return E_FAIL;

	if (true == m_bGravity)
	{
		if (nullptr == m_pTransformCom)
			return 0;

		m_pTransformCom->Free_Fall(_fTimeDelta);
	}

	return S_OK;
}

HRESULT CPushBox::Collision_ToPlayer()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCollision_CubeSphere* pCubeSphereCollision
		= (CCollision_CubeSphere*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Collision_CubeSphere");
	if (nullptr == pCubeSphereCollision)
		return E_FAIL;

	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
	{
		//m_bGravity = false;

		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
		if (nullptr == pPlayerTransform)
			return E_FAIL;

		if (pPlayerTransform->Get_TransformDesc().vPosition.y > vPos.y + vHalfScale.y)
		{
			//플레이어가 상자 위에 있을 때 충돌 후 공중에 체류하는 것을 방지하기 위한 코드
			vInterval.y *= 1.0001f;
			pPlayerTransform->Go_Posion(-vInterval);
			/*사운드*/
			pManagement->StopSound(CSound_Manager::PLAYER_JUMP);
		}
		else/* if(pPlayerTransform->Get_TransformDesc().vPosition.x > vPos.x + vHalfScale.x || pPlayerTransform->Get_TransformDesc().vPosition.x < vPos.x - vHalfScale.x)*/
		{
			//속도 느려짐 및 이중 충돌 발생시 캐릭터가 관통하는 버그를 위한 수정
			vInterval.x *= 0.5001f;
			m_pTransformCom->Go_Posion(vInterval);
			pPlayerTransform->Go_Posion(-vInterval);
		}

		if (vInterval.y > 0.f)
		{
			CTransform::TRANSFORM_DESC tPlayer_TransformDesc = pPlayerTransform->Get_TransformDesc();
			tPlayer_TransformDesc.vVelocity.y = -0.1f;
			pPlayerTransform->Set_TransformDesc(tPlayer_TransformDesc);
		}
		else if (vInterval.y < 0.f)
			pPlayerTransform->ClearVelocity();
	}

	return S_OK;
}

HRESULT CPushBox::Collision_ToTile(float _fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTile* pTile = nullptr;
	CCollision_Cube* pTileCollision = nullptr;
	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Cube");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pTile = (CTile*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Cube", i);
		if (nullptr == pTile)
			return E_FAIL;

		CTransform* pTile_Transform = (CTransform*)pTile->Find_Component(L"Com_Transform");
		if (nullptr == pTile_Transform)
			return E_FAIL;

		CTransform::TRANSFORM_DESC tTile_TransformDesc = pTile_Transform->Get_TransformDesc();
		D3DXVECTOR3 vTilePos = tTile_TransformDesc.vPosition;
		D3DXVECTOR3 vTileHalfSize = tTile_TransformDesc.vScale *0.5f;
		float fLength = D3DXVec3Length(&(vTilePos - vPos));

		if (3.f > fLength)
		{
			// 충돌 검사
			pTileCollision = (CCollision_Cube*)pTile->Find_Component(L"Com_Collision_Cube");
			if (nullptr == pTileCollision)
				return E_FAIL;

			if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
			{
				m_bGravity = false;
				if (vTilePos.x - vTileHalfSize.x < vPos.x && vTilePos.x + vTileHalfSize.x > vPos.x && vTilePos.y > vPos.y)
				{
					vPos.y -= 0.001f;
					m_pTransformCom->Set_Position(vPos);
				}
				vInterval.x *= 1.0001f;
				m_pTransformCom->ClearVelocity();
				m_pTransformCom->Go_Posion(-vInterval);
			}
			else
				continue;
		}
		else
			continue;
	}

	return S_OK;
}

HRESULT CPushBox::Collision_ToPushBox()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// 상대 PushBox
	CPushBox* pPushBox = nullptr;
	CCollision_Cube* pBoxCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_PushBox");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pPushBox = (CPushBox*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_PushBox", i);
		if (nullptr == pPushBox)
			return E_FAIL;

		if (this == pPushBox)
			continue;

		if (pPushBox->Get_Color() == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			continue;

		pBoxCollision = (CCollision_Cube*)pPushBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return E_FAIL;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
		{
			m_bGravity = false;

			CTransform* pPushBox_Transform = (CTransform*)pPushBox->Find_Component(L"Com_Transform");
			if (nullptr == pPushBox_Transform)
				return E_FAIL;

			vInterval *= 1.00001f;
			m_pTransformCom->Go_Posion(-vInterval);
			pPushBox_Transform->Go_Posion(vInterval);
			pPushBox_Transform->ClearVelocity();

			return S_OK;
		}
		else
			continue;
	}

	return S_OK;
}

CPushBox * CPushBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPushBox* pInstance = new CPushBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CPushBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPushBox::Clone_GameObject(void * pArg)
{
	CPushBox* pInstance = new CPushBox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CPushBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPushBox::Free()
{
	Safe_Release(m_pCubeCollisionCom);

	CMyObject::Free();
}
