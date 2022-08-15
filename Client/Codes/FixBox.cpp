#include "stdafx.h"
#include "..\Headers\FixBox.h"

#include "PushBox.h"
#include "Player_Observer.h"

USING(Client)

CFixBox::CFixBox(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CFixBox::CFixBox(const CFixBox & other)
	: CMyObject(other)
{
}

HRESULT CFixBox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFixBox::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CFixBox::Update_GameObject(float fTimeDelta)
{
	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CFixBox::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CFixBox::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CFixBox::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_FixBox",
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

HRESULT CFixBox::SetUp_CollisionDesc()
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

HRESULT CFixBox::Collision(float _fTimeDelta)
{
	if (FAILED(Collision_ToPlayer()))
		return E_FAIL;

	if (FAILED(Collision_ToPushBox()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFixBox::Collision_ToPlayer()
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
		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
		if (nullptr == pPlayerTransform)
			return E_FAIL;
		if (pPlayerTransform->Get_TransformDesc().vPosition.y > m_pTransformCom->Get_TransformDesc().vPosition.y)
		{
			//플레이어가 상자 위에 있을 때 충돌 후 공중에 체류하는 것을 방지하기 위한 코드
			vInterval.y *= 0.99f;
			pPlayerTransform->Go_Posion(-vInterval);

			/*사운드*/
			pManagement->StopSound(CSound_Manager::PLAYER_JUMP);
		}
		else
			pPlayerTransform->Go_Posion(-vInterval);
		
		if (vInterval.y > 0.f)
		{
			// 플레이어 머리와 충돌할 경우 속도가 0.f이 되어 플레이어의 상태가 PLAYER_IDLE 상태로 돌아가는 것을 방지함.
			CTransform::TRANSFORM_DESC tPlayer_TransformDesc = pPlayerTransform->Get_TransformDesc();
			tPlayer_TransformDesc.vVelocity.y = -0.1f;
			pPlayerTransform->Set_TransformDesc(tPlayer_TransformDesc);
		}
		else if (vInterval.y < 0.f)
			pPlayerTransform->ClearVelocity();
	}

	return S_OK;
}

HRESULT CFixBox::Collision_ToPushBox()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

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

		pBoxCollision = (CCollision_Cube*)pPushBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return E_FAIL;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
		{
			CTransform* pPushBox_Transform = (CTransform*)pPushBox->Find_Component(L"Com_Transform");
			if (nullptr == pPushBox_Transform)
				return E_FAIL;

			pPushBox_Transform->Go_Posion(vInterval);
			pPushBox_Transform->ClearVelocity();

			return S_OK;
		}
		else
			continue;
	}

	return S_OK;
}

CFixBox * CFixBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFixBox* pInstance = new CFixBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CFixBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFixBox::Clone_GameObject(void * pArg)
{
	CFixBox* pInstance = new CFixBox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CFixBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFixBox::Free()
{
	Safe_Release(m_pCubeCollisionCom);

	CMyObject::Free();
}
