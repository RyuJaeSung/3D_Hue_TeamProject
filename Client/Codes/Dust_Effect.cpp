#include "stdafx.h"
#include "Dust_Effect.h"

USING(Client)

CDust_Effect::CDust_Effect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CDust_Effect::CDust_Effect(const CDust_Effect & other)
	: CGameObject(other)
{
}

HRESULT CDust_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDust_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 위치여기서 초기화
	// 리소스 추가하기...
	m_pTransformCom->Set_TransformDesc(*((CTransform::TRANSFORM_DESC*)pArg));

	m_iIndex = rand() % 6;
	return S_OK;
}

int CDust_Effect::Update_GameObject(float fTimeDelta)
{
	// 여기서 움직임? 크기 등등의 변경을 구현
	if (FAILED(Movement(fTimeDelta)))
		return 0;
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;


	return 0;
}

int CDust_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CDust_Effect::Render_GameObject()
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

HRESULT CDust_Effect::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Dust_Effect",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 5.f, 1.f, 5.f };
	TransformInfo.vScale = { 1.f, 1.f, 1.f };
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CDust_Effect::Movement(float fTimeDelta)
{
	// 돌면서 작아지면서 굴러가다가 크기가 0보다 작아지면 죽인다.

	float fNumber = m_pTransformCom->Get_TransformDesc().fSpeedPerSec;
	float fDir = 0.01f;
	if (fNumber < 0)
		fDir *= -1.f;

	m_pTransformCom->Set_Scale_Down(0.985f);
	m_pTransformCom->Set_Rotate(-0.01f, CTransform::AXIS_Z);
	m_pTransformCom->Set_Pos(fDir * fTimeDelta * 50.f, CTransform::AXIS_X);
	if (m_pTransformCom->Get_TransformDesc().vScale.x < 0.1f)
		m_IsAlived = false;

	return S_OK;
}

CDust_Effect * CDust_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDust_Effect* pInstance = new CDust_Effect(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Create CDust_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDust_Effect::Clone_GameObject(void * pArg)
{
	CDust_Effect* pInstance = new CDust_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CDust_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDust_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();

}