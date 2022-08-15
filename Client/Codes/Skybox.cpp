#include "stdafx.h"
#include "..\Headers\Skybox.h"
#include "Player_Observer.h"
#include "Camera.h"

USING(Client)

CSkybox::CSkybox(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int i = 0; i < SKY_END; ++i)
		m_pTransformCom[i] = nullptr;
}

CSkybox::CSkybox(const CSkybox & other)
	: CGameObject(other)
{
	for (int i = 0; i < SKY_END; ++i)
		m_pTransformCom[i] = nullptr;
}

HRESULT CSkybox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkybox::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pObserver = new CPlayer_Observer;
	CManagement::Get_Instance()->Subscribe(m_pObserver);

	m_pTransformCom[SKY_BACK]->Set_Scale(D3DXVECTOR3(10.f, 10.f, 10.f));

	m_pTransformCom[SKY_CLOUD]->Set_Scale(D3DXVECTOR3(8.f, 8.f, 8.f));

	m_pTransformCom[SKY_MOUNT]->Set_Scale(D3DXVECTOR3(5.f, 5.f, 5.f));
	return S_OK;
}

int CSkybox::Update_GameObject(float fTimeDelta)
{
	m_dwColor = dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color();
	if (FAILED(Movement(fTimeDelta)))
		return 0;

	for (int i = 0; i < SKY_END; ++i)
	{
		if (FAILED(m_pTransformCom[i]->Update_Transform()))
			return 0;
	}

	return 0;
}

int CSkybox::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_PRIORITY, this)))
		return 0;

	return 0;
}

HRESULT CSkybox::Render_GameObject()
{
	// 알파블랜딩
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파블렌딩 활성화 on.

	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 색깔 넣어주기
		m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE); // Zread를 하지 않겠다. 기존에 쓰여있던 Z값이 나보다 작건 크건 상관 없이 무조건 그리겠다.
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Zwrite를 하지 않겠다. 나의 Z값을 기록하지 않으므로 추후에 그려질 대상이 내 Z값과 비교할 수 없다(나를 덮어 쓸 것이다).

	for (int i = 0; i < SKY_END; ++i)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom[i]->Get_TransformDesc().matWorld);

		if (FAILED(m_pTextureCom->Set_Texture(i)))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	}

	//// 장치의 디폴트 값.
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 색 원래대로
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파블렌딩 활성화 off.

	return S_OK;
}

HRESULT CSkybox::Add_Component()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
		//For.Com_Texture
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox",
			L"Com_Texture", (CComponent**)&m_pTextureCom)))
			return E_FAIL;
#pragma endregion

#pragma region Com_Transform
		//For.Com_Transform

		TCHAR szBuff[32] = L"";
		CTransform::TRANSFORM_DESC TransformInfo;

		for (int i = 0; i < SKY_END; ++i)
		{
			wsprintf(szBuff, L"Com_Transform%d", i);

			TransformInfo.vPosition = { 0.f, 0.f, 0.f };

			switch (i)
			{
			case SKY_BACK:
				TransformInfo.fRadianPerSec = 0.005f; // 사실 0이어도 됨..
				break;
			case SKY_CLOUD:
				TransformInfo.fRadianPerSec = 0.02f;
				break;
			case SKY_MOUNT:
				TransformInfo.fRadianPerSec = 0.1f;
				break;
			}

			if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
				szBuff, (CComponent**)&m_pTransformCom[i], &TransformInfo)))
				return E_FAIL;
		}
#pragma endregion

	return S_OK;
}

HRESULT CSkybox::Movement(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	// 카메라 위치에 따라 위치 조정
	for (int i = 0; i < SKY_END; ++i)
	{
		if (nullptr == m_pTransformCom[i])
			return E_FAIL;

		m_pTransformCom[i]->Set_Position(pCamera->Get_CameraDesc().vEye);

		// 플레이어가 이동하면 더 빠르게 회전해야함.
		m_pTransformCom[i]->Rotate_Axis(CTransform::AXIS_Y, fTimeDelta);
	}

	return S_OK;
}

CSkybox* CSkybox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkybox* pInstance = new CSkybox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CSkybox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkybox::Clone_GameObject(void * pArg)
{
	CSkybox* pInstance = new CSkybox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CSkybox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkybox::Free()
{
	//
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	for (int i = 0 ; i < SKY_END; ++i)
		Safe_Release(m_pTransformCom[i]);
	Safe_Release(m_pObserver);
	//
	CGameObject::Free();
}
