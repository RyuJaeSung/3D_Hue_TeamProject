#include "stdafx.h"
#include "Item_Effect.h"

USING(Client)

CItem_Effect::CItem_Effect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CItem_Effect::CItem_Effect(const CItem_Effect & other)
	: CGameObject(other)
{

}

HRESULT CItem_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItem_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 위치여기서 초기화
	// 리소스 추가하기...
	m_pTransformCom->Set_TransformDesc(*((CTransform::TRANSFORM_DESC*)pArg));
	// 생성좌표
	m_vDir = { (float)(rand() % 10 - 5),(float)(rand() % 10 - 5), 0.f };
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

int CItem_Effect::Update_GameObject(float fTimeDelta)
{
	// 여기서 움직임? 크기 등등의 변경을 구현
	if (FAILED(Movement(fTimeDelta)))
		return 0;
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CItem_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CItem_Effect::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	// 0~5 까지 선택
	if (FAILED(m_pTextureCom->Set_Texture(m_iIndex)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CItem_Effect::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_Effect",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 5.f, 1.f, 5.f };
	TransformInfo.vScale = { 0.1f, 0.1f, 0.1f };
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CItem_Effect::Movement(float fTimeDelta)
{
	// 주어진 방향으로 이동하다가
	// 크기가 일정미만이면 죽인다.
	m_pTransformCom->Set_Scale_Down(0.95f);
	m_pTransformCom->Set_Pos(m_vDir.x * fTimeDelta, CTransform::AXIS_X);
	m_pTransformCom->Set_Pos(m_vDir.y * fTimeDelta, CTransform::AXIS_Y);

	if (m_pTransformCom->Get_TransformDesc().vScale.x < 0.005f)
		m_IsAlived = false;

	return S_OK;
}

CItem_Effect * CItem_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Effect* pInstance = new CItem_Effect(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Create CItem_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Effect::Clone_GameObject(void * pArg)
{
	CItem_Effect* pInstance = new CItem_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CItem_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();

}
