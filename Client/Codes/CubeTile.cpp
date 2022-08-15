#include "stdafx.h"
#include "..\Headers\CubeTile.h"

USING(Client)

CCubeTile::CCubeTile(LPDIRECT3DDEVICE9 pDevice)
	: CTile(pDevice)
{
}

CCubeTile::CCubeTile(const CCubeTile & other)
	: CTile(other)
{
}

HRESULT CCubeTile::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCubeTile::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CCubeTile::Update_GameObject(float fTimeDelta)
{
	return 	CTile::Update_GameObject(fTimeDelta);
}

int CCubeTile::LateUpdate_GameObject(float fTimeDelta)
{
	return CTile::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CCubeTile::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeTile::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	//For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 3.f, 0.f, 0.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Cube",
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

CCubeTile * CCubeTile::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTile* pInstance = new CCubeTile(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCubeTile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCubeTile::Clone_GameObject(void * pArg)
{
	CCubeTile* pInstance = new CCubeTile(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CCubeTile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeTile::Free()
{
	Safe_Release(m_pVIBufferCom);

	CTile::Free();
}
