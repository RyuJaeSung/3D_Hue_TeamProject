#include "stdafx.h"
#include "..\Headers\Terrain.h"

USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CTerrain::CTerrain(const CTerrain & other)
	: CGameObject(other)
{
}

HRESULT CTerrain::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CTerrain::Update_GameObject(float fTimeDelta)
{
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CTerrain::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_NONALPHA, this)))
		return 0;

	return 0;
}

HRESULT CTerrain::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CTerrain::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_VIBuffer_TerrainTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Terrain",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	//CTransform::TRANSFORM_DESC TransformInfo;
	//TransformInfo.vPosition = { 10.f, 0.f, 10.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone_GameObject(void * pArg)
{
	CTerrain* pInstance = new CTerrain(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	//
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	//
	CGameObject::Free();
}
