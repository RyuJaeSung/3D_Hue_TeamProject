#include "stdafx.h"
#include "Stone_Effect.h"

USING(Client)

CStone_Effect::CStone_Effect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CStone_Effect::CStone_Effect(const CStone_Effect & other)
	:CGameObject(other)
	,m_fAngle(0.f)
{
}

HRESULT CStone_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStone_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	// 플레이어 위치 받아서 초기 좌표 셋팅
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return E_FAIL;
	m_pTransformCom->Set_Position(pPlayerTransform->Get_TransformDesc().vPosition);

	// 플레이어 위치에서 부터 얼마나 떨어진데에 만들지 좌표 넘겨주기

	//// pArg는 Transform임. 위치만 뽑아서 넣음
	m_pTransformCom->Set_Pos(((CTransform::TRANSFORM_DESC*)pArg)->vPosition.y, CTransform::AXIS_Y);

	return S_OK;
} 

int CStone_Effect::Update_GameObject(float fTimeDelta)
{
	// 여기서 움직임? 크기 등등의 변경을 구현
	if (FAILED(Movement(fTimeDelta)))
		return 0;
	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;


	return 0;
}

int CStone_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CStone_Effect::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CStone_Effect::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Stone_Effect",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 5.f, 1.f, 5.f };
	TransformInfo.vScale = { 0.05f, 0.05f, 0.05f };
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CStone_Effect::Movement(float fTimeDelta)
{
	// 플레이어의 이동방향 받아와야함
	// 이동방향의 반대 방향으로 x가 이동

	if (m_fAngle >= 2.f)
	{
		m_IsAlived = false;
		return S_OK;
	}
	m_fAngle += fTimeDelta*3.f;

	//코사인 그리다가 사라짐
	float fY = cosf(m_fAngle)*0.001f;
	m_pTransformCom->Set_Pos(fY, CTransform::AXIS_Y);
	if(LEFT == m_eDir)
		m_pTransformCom->Set_Pos(-fTimeDelta * 0.8f, CTransform::AXIS_X);
	else
		m_pTransformCom->Set_Pos(fTimeDelta * 0.8f, CTransform::AXIS_Y);

	return S_OK;
}

CStone_Effect * CStone_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStone_Effect* pInstance = new CStone_Effect(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Create CStone_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStone_Effect::Clone_GameObject(void * pArg)
{
	CStone_Effect* pInstance = new CStone_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CStone_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStone_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();

}
