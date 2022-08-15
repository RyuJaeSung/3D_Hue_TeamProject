#include "stdafx.h"
#include "..\Headers\Pet_Item_Effect.h"

USING(Client)

CPet_Item_Effect::CPet_Item_Effect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CPet_Item_Effect::CPet_Item_Effect(const CPet_Item_Effect & other)
	: CGameObject(other)
{

}

HRESULT CPet_Item_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPet_Item_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 위치여기서 초기화
	// 리소스 추가하기...
	m_pTransformCom->Set_TransformDesc(*((CTransform::TRANSFORM_DESC*)pArg));

	return S_OK;
}

int CPet_Item_Effect::Update_GameObject(float fTimeDelta)
{
	if (m_eItemStatus == ITEM_EFFECT_DEAD)
		m_IsAlived = false;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	// 여기서 움직임? 크기 등등의 변경을 구현
	if (FAILED(Movement(fTimeDelta)))
		return 0;
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CPet_Item_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CPet_Item_Effect::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	if (m_eItemStatus != ITEM_EFFECT_STOP)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파블렌딩 활성화 on
		// 0~5 까지 선택
		if (FAILED(m_pTextureCom->Set_Texture(m_iIndex)))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파블렌딩 활성화 off.
	}
	return S_OK;
}

HRESULT CPet_Item_Effect::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Pet_Item_Effect",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CPet_Item_Effect::Movement(float fTimeDelta)
{
	if (!m_bReverse)
	{
		m_pTransformCom->Set_Scale_Down(1.02f);

		if (m_pTransformCom->Get_TransformDesc().vScale.x > 0.45f)
			m_bReverse = true;
	}
	else if (m_bReverse)
	{
		m_pTransformCom->Set_Scale_Down(0.95f);

		if (m_pTransformCom->Get_TransformDesc().vScale.x < 0.35f)
			m_bReverse = false;
	}

	return S_OK;
}

CPet_Item_Effect * CPet_Item_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPet_Item_Effect* pInstance = new CPet_Item_Effect(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Create CPet_Item_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPet_Item_Effect::Clone_GameObject(void * pArg)
{
	CPet_Item_Effect* pInstance = new CPet_Item_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CPet_Item_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPet_Item_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
