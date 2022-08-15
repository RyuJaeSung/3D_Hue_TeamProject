#include "stdafx.h"
#include "..\Headers\BackGround.h"
#include "Texture.h"

USING(Client)

CBackGround::CBackGround(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CBackGround::CBackGround(const CBackGround & _other)
	: CGameObject(_other)
	,m_pTransformCom(_other.m_pTransformCom)
{
}

const bool CBackGround::Get_NextScene() const
{
	return m_IsNextScene;
}

void CBackGround::Set_SceneTexType(const SCENE_TEX _eTexType)
{
	m_eSceneTexType = _eTexType;
}

HRESULT CBackGround::Set_Transform(D3DXVECTOR3 & _vPos, D3DXVECTOR3 & _vSacle)
{
	if (!_vPos || !_vSacle)
		return E_FAIL;
	m_pTransformCom->Set_Position(_vPos);
	m_pTransformCom->Set_Scale(_vSacle);

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	return S_OK;
}

HRESULT CBackGround::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Ready_GameObject(void * pArg)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	pManagement->Set_OrthoMatrix(WINCX, WINCY);
#pragma region Com_RectTexture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer_RectTexture", (CComponent**)&m_RectTextureCom)))
		return E_FAIL;
#pragma endregion
#pragma region Com_Transform
	CTransform::tagTransformDesc TransformInfo;
	TransformInfo.vScale = { WINCX, WINCY, 0.f };
	TransformInfo.vPosition = { 0.f, 0.f, 0.f };
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

int CBackGround::Update_GameObject(float fTimeDelta)
{
	if (m_iCount < 20)
	{
		m_fx += m_fSpeed;
		m_fy += m_fSpeed;
		m_iCount++;
	}
	else
		m_IsNextScene = true;

	return 0;
}

int CBackGround::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_UI, this)))
		return 0;

	return 0;
}

HRESULT CBackGround::Render_GameObject()
{
	if (m_eSceneTexType == TEX_END)
		return E_FAIL;
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	Set_Transform(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(WINCX , WINCY, 0.f));
	if (FAILED(m_pTextureCom[m_eSceneTexType]->Set_Texture(0)))
		return E_FAIL;
	if (FAILED(m_RectTextureCom->Render_VIBuffer()))
		return E_FAIL;
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, FALSE);
		Render_Font();
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	if(m_iCount >= 20)
		m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	return S_OK;
}

HRESULT CBackGround::Add_Component()
{
#pragma region Com_Texture_Logo_BackGround
	if (FAILED(CGameObject::Add_Component(SCENE_LOGO, L"Component_Logo_BackGround",
		L"Com_Logo_BackGround", (CComponent**)&m_pTextureCom[TEX_LOGO])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture_Logo_BackGround
	if (FAILED(CGameObject::Add_Component(SCENE_LOGO, L"Component_Logo_Font",
		L"Com_Logo_Font", (CComponent**)&m_pTextureCom[TEX_FONT])))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CBackGround::Render_Font()
{
		//P 
		Set_Transform(D3DXVECTOR3(-410.f + (m_fx + 6.6f), 0.f, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(0)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//A
		Set_Transform(D3DXVECTOR3(-340.f + m_fx, -300.f + m_fy, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(1)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//L
		Set_Transform(D3DXVECTOR3(-75.f, 300.f - m_fy * 2.f, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(2)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//E
		Set_Transform(D3DXVECTOR3(-15.f, -100.f, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(3)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//T
		Set_Transform(D3DXVECTOR3(250.f - m_fx, 100.f - m_fy, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(4)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//T
		Set_Transform(D3DXVECTOR3(315.f - m_fx, -500.f + m_fy * 2.f, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(5)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;
		//E
		Set_Transform(D3DXVECTOR3(380.f - (m_fx + 6.6f), 0.f, 0.f), D3DXVECTOR3(64.f, 128.f, 0.f));
		if (FAILED(m_pTextureCom[TEX_FONT]->Set_Texture(6)))
			return E_FAIL;
		if (FAILED(m_RectTextureCom->Render_VIBuffer()))
			return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBackGround* pInstance = new CBackGround(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CBackGround::Clone_GameObject(void * pArg)
{
	CBackGround* pInstance = new CBackGround(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBackGround::Free()
{
	for (int i = 0; i < TEX_END; ++i)
		Safe_Release(m_pTextureCom[i]);
	Safe_Release(m_RectTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}