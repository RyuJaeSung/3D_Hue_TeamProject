#include "stdafx.h"
#include "..\Headers\Item.h"

#include "Scene_Stage.h"
#include "Static_Camera.h"
#include "Player_Observer.h"
#include "Item_Inven.h"
#include "Pet_Item_Effect.h"

USING(Client)

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CItem::CItem(const CItem & other)
	: CMyObject(other)
{
}

HRESULT CItem::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItem::Ready_GameObject(void * pArg)
{
	for (int i = 0; i < TEXTURE_END; ++i)
	{
		m_pItemVIBufferCom[i] = nullptr;
		m_pItemTextureCom[i] = nullptr;
	}

	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_Item_Desc, pArg, sizeof(ITEM_DESC));

	return S_OK;
}

int CItem::Update_GameObject(float fTimeDelta)
{
	if (m_bInit)
	{
		m_IsAlived = true;
		Set_TextureColor(m_Item_Desc.eColorType);
		m_pTransformCom->Set_Position(m_Item_Desc.vItemPos);

		if (FAILED(m_pTransformCom->Update_Transform()))
			return 0;

		Create_Item_Effect();
		m_bInit = false;
	}

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CItem::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (m_dwColor == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
		dynamic_cast<CPet_Item_Effect*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Pet_Item_Effect"))->Set_Item_Effect_Status(CPet_Item_Effect::ITEM_EFFECT_STOP);
	else
		dynamic_cast<CPet_Item_Effect*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Pet_Item_Effect"))->Set_Item_Effect_Status(CPet_Item_Effect::ITEM_EFFECT_PLAY);

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CItem::Render_GameObject()
{
	// 아이템 배경을 그린다
	D3DXMATRIX matBg = m_pTransformCom->Get_TransformDesc().matWorld;
	matBg._11 += 0.03f;
	matBg._22 += 0.03f;
	m_pDevice->SetTransform(D3DTS_WORLD, &matBg);

	if (FAILED(m_pItemTextureCom[TEXTURE_BG]->Set_Texture(0)))
		return E_FAIL;
	if (FAILED(m_pItemVIBufferCom[TEXTURE_BG]->Render_VIBuffer()))
		return E_FAIL;

	// 아이템을 그린다.
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	if (FAILED(CMyObject::Render_GameObject()))
		return E_FAIL;

	// 고글을 그린다
	D3DXMATRIX matGlass = m_pTransformCom->Get_TransformDesc().matWorld;
	matGlass._43 -= 0.051f;
	m_pDevice->SetTransform(D3DTS_WORLD, &matGlass);

	if (FAILED(m_pItemTextureCom[TEXTURE_GLASS]->Set_Texture(0)))
		return E_FAIL;
	if (FAILED(m_pItemVIBufferCom[TEXTURE_GLASS]->Render_VIBuffer()))
		return E_FAIL;
	//

	return S_OK;
}

HRESULT CItem::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer_Glass", (CComponent**)&m_pItemVIBufferCom[TEXTURE_GLASS])))
		return E_FAIL;
#pragma endregion

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer_Bg", (CComponent**)&m_pItemVIBufferCom[TEXTURE_BG])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vScale = { 0.3f, 0.3f, 0.1f };
	TransformInfo.vRotate = { 0.f, 0.f, 0.f };
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_Glass",
		L"Com_Texture_Glass", (CComponent**)&m_pItemTextureCom[TEXTURE_GLASS])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_Bg",
		L"Com_Texture_Bg", (CComponent**)&m_pItemTextureCom[TEXTURE_BG])))
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

HRESULT CItem::Collision(float _fTimeDelta)
{
	return Collision_ToPlayer(_fTimeDelta);
}

HRESULT CItem::Collision_ToPlayer(float _fTimeDelta)
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
		CItem_Inven* pItem_Inven = (CItem_Inven*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Item_Inven");
		if (nullptr == pItem_Inven)
			return 0;
		
		pItem_Inven->Set_ItemState_True((CItem_Inven::ITEM_COLOR)((int)m_eColorType - 1));

		/*사운드*/
		pManagement->Play_Sound(L"a_hue Item.ogg", CSound_Manager::ITEM);

		m_IsAlived = false;
		dynamic_cast<CPet_Item_Effect*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Pet_Item_Effect"))->Set_Item_Effect_Status(CPet_Item_Effect::ITEM_EFFECT_DEAD);
	}

	return S_OK;
}

HRESULT CItem::Create_Item_Effect()
{
	CTransform::TRANSFORM_DESC TransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);
	TransformDesc.vPosition = vPos;
	Ready_Layer_Effect(L"GameObject_Pet_Item_Effect", L"Layer_Pet_Item_Effect", TransformDesc);

	return S_OK;
}

HRESULT CItem::Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// 위치만 세팅해서 넘긴다

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, GameObjectTag, pManagement->Get_Current_SceneID(), LayerTag, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem* pInstance = new CItem(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone_GameObject(void * pArg)
{
	CItem* pInstance = new CItem(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	for (int i = 0; i < TEXTURE_END; ++i)
	{
		Safe_Release(m_pItemTextureCom[i]);
		Safe_Release(m_pItemVIBufferCom[i]);
	}

	CMyObject::Free();
}