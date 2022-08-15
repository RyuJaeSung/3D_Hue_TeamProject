#include "stdafx.h"
#include "..\Headers\Ladder.h"


USING(Client)

CLadder::CLadder(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CLadder::CLadder(const CLadder & other)
	: CMyObject(other)
{
}

HRESULT CLadder::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLadder::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CLadder::Update_GameObject(float fTimeDelta)
{
	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CLadder::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CLadder::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CLadder::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Ladder",
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

HRESULT CLadder::SetUp_CollisionDesc()
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

CLadder * CLadder::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLadder* pInstance = new CLadder(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLadder::Clone_GameObject(void * pArg)
{
	CLadder* pInstance = new CLadder(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder::Free()
{
	Safe_Release(m_pCubeCollisionCom);

	CMyObject::Free();
}
