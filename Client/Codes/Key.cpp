#include "stdafx.h"
#include "..\Headers\Key.h"

#include "Scene_Stage.h"
#include "Door.h"
#include "Player.h"
#include "Static_Camera.h"
#include "Player_Observer.h"

USING(Client)

CKey::CKey(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CKey::CKey(const CKey & other)
	: CMyObject(other)
{
}

HRESULT CKey::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CKey::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CKey::Update_GameObject(float fTimeDelta)
{
	return CMyObject::Update_GameObject(fTimeDelta);
}

int CKey::LateUpdate_GameObject(float fTimeDelta)
{
	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
		Create_Item_Effect();

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CKey::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CKey::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Key",
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

HRESULT CKey::Collision(float _fTimeDelta)
{
	return Collision_ToPlayer(_fTimeDelta);
}

HRESULT CKey::Collision_ToPlayer(float _fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// 캐릭터가 사망한 상태라면 키와 충돌하지 않음!
	if (CPlayer::PLAYER_DEAD == dynamic_cast<CPlayer*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player")))->Get_State())
		return E_FAIL;

	CCollision_CubeSphere* pCubeSphereCollision
		= (CCollision_CubeSphere*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Collision_CubeSphere");
	if (nullptr == pCubeSphereCollision)
		return E_FAIL;

	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
	{
		/*사운드*/
		pManagement->Play_Sound(L"a_hue key collective.ogg", CSound_Manager::KEY);

		CStatic_Camera* pCamera = (CStatic_Camera*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Camera");
		if (nullptr == pCamera)
			return E_FAIL;

		pCamera->Change_Camera_Effect(CAM_EFFECT_REVOLUTION_Y, _fTimeDelta, vPos);

		m_IsAlived = false;

		CScene_Stage* pCurScene = (CScene_Stage*)pManagement->Get_CurrentScene_Pointer();
		if (nullptr == pCurScene)
			return E_FAIL;

		pCurScene->Set_StageKey(true);

		CDoor* pDoor = nullptr;
		int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Door");

		for (int i = 0; i < iLayerSize; ++i)
		{
			pDoor = (CDoor*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Door", i);

			if (CDoor::DOOR_NEXT == pDoor->Get_DoorType())
			{
				pDoor->Set_TextureIndex(1); // 문 활성화
				return S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CKey::Create_Item_Effect()
{
	CTransform::TRANSFORM_DESC TransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);
	TransformDesc.vPosition = vPos;
	Ready_Layer_Effect(L"GameObject_Item_Effect", L"Layer_Item_Effect", TransformDesc);

	return S_OK;
}

HRESULT CKey::Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// 위치만 세팅해서 넘긴다

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, GameObjectTag, pManagement->Get_Current_SceneID(), LayerTag, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CKey * CKey::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CKey* pInstance = new CKey(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKey::Clone_GameObject(void * pArg)
{
	CKey* pInstance = new CKey(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKey::Free()
{
	CMyObject::Free();
}