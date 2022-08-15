#include "stdafx.h"
#include "..\Headers\Laser_Effect.h"

#include "Player_Observer.h"

USING(Client)

CLaser_Effect::CLaser_Effect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CLaser_Effect::CLaser_Effect(const CLaser_Effect & other)
	: CGameObject(other)
{
}

void CLaser_Effect::Set_TextureColor(const COLOR_TYPE & _eColorType)
{
	m_eColorType = _eColorType;

	SetUp_TextureColor(); // 색상 타입에 따라 DWORD 색상 결정
}

HRESULT CLaser_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLaser_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pObserver = new CPlayer_Observer;
	CManagement::Get_Instance()->Subscribe(m_pObserver);

	// 위치여기서 초기화
	// 리소스 추가하기...
	m_pTransformCom->Set_TransformDesc(*((CTransform::TRANSFORM_DESC*)pArg));

	return S_OK;
}

int CLaser_Effect::Update_GameObject(float fTimeDelta)
{
	// 여기서 움직임? 크기 등등의 변경을 구현
	if (FAILED(Movement(fTimeDelta)))
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CLaser_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
			return 0;
	}
	return 0;
}

HRESULT CLaser_Effect::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	// 색상 바꾸는 함수
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	// 0~5 까지 선택
	if (FAILED(m_pTextureCom->Set_Texture(m_iIndex)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 색 원래대로
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CLaser_Effect::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CLaser_Effect::Movement(float fTimeDelta)
{
	// 주어진 방향으로 이동하다가
	// 크기가 일정미만이면 죽인다.
	m_pTransformCom->Set_Scale_Down(0.99f);
	m_pTransformCom->Set_Pos(m_vDir.x * fTimeDelta, CTransform::AXIS_X);
	m_pTransformCom->Set_Pos(m_vDir.y * fTimeDelta, CTransform::AXIS_Y);

	if (m_pTransformCom->Get_TransformDesc().vScale.x < 0.005f)
		m_IsAlived = false;

	return S_OK;
}

void CLaser_Effect::SetUp_TextureColor()
{
	switch (m_eColorType)
	{
	case COLOR_BLACK:
		m_dwColor = RGBA_BLACK;
		break;
	case COLOR_RED:
		m_dwColor = RGBA_RED;
		break;
	case COLOR_ORANGE:
		m_dwColor = RGBA_ORANGE;
		break;
	case COLOR_YELLOW:
		m_dwColor = RGBA_YELLOW;
		break;
	case COLOR_GREEN:
		m_dwColor = RGBA_GREEN;
		break;
	case COLOR_BLUE:
		m_dwColor = RGBA_BLUE;
		break;
	case COLOR_NAVY:
		m_dwColor = RGBA_NAVY;
		break;
	case COLOR_PURPLE:
		m_dwColor = RGBA_PURPLE;
		break;
	case COLOR_PINK:
		m_dwColor = RGBA_PINK;
		break;
	case COLOR_WHITE:
		m_dwColor = RGBA_WHITE;
		break;
	}
}

CLaser_Effect * CLaser_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLaser_Effect* pInstance = new CLaser_Effect(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Create CLaser_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaser_Effect::Clone_GameObject(void * pArg)
{
	CLaser_Effect* pInstance = new CLaser_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CLaser_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLaser_Effect::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pObserver);

	CGameObject::Free();
}
