#include "stdafx.h"
#include "..\Headers\Door.h"

#include "Scene_Stage.h"

USING(Client)

CDoor::CDoor(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CDoor::CDoor(const CDoor & other)
	: CMyObject(other)
{
}

HRESULT CDoor::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CDoor::Update_GameObject(float fTimeDelta)
{
	return CMyObject::Update_GameObject(fTimeDelta);
}

int CDoor::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CDoor::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CDoor::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Door",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

HRESULT CDoor::Collision(float _fTimeDelta)
{
	return Collision_ToPlayer();
}

HRESULT CDoor::Collision_ToPlayer()
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

	CScene_Stage* pCurScene = (CScene_Stage*)pManagement->Get_CurrentScene_Pointer();
	if (nullptr == pCurScene)
		return E_FAIL;

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
	{
		if (true == pCurScene->Get_StageKey()
			&& DOOR_NEXT == m_eDoorType)
			pCurScene->Set_NextStage(true);
	}
	else
		pCurScene->Set_NextStage(false);

	return S_OK;
}

CDoor * CDoor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDoor* pInstance = new CDoor(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDoor::Clone_GameObject(void * pArg)
{
	CDoor* pInstance = new CDoor(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	CMyObject::Free();
}
