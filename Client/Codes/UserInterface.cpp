#include "stdafx.h"
#include "..\Headers\UserInterface.h"

USING(Client)

CUserInterface::CUserInterface(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
	Safe_AddRef(m_pVIBufferCom);
	Safe_AddRef(m_pTextureCom);
}

CUserInterface::CUserInterface(const CUserInterface & _other)
	:CGameObject(_other)
{
	Safe_AddRef(_other.m_pVIBufferCom);
	Safe_AddRef(_other.m_pTextureCom);
}

HRESULT CUserInterface::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CUserInterface::Ready_GameObject(void * pArg)
{
	return S_OK;
}

int CUserInterface::Update_GameObject(float fTimeDelta)
{
	return 0;
}

int CUserInterface::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_UI, this)))
		return 0;

	return 0;
}

HRESULT CUserInterface::Render_GameObject()
{
	return S_OK;
}

HRESULT CUserInterface::Add_Component()
{
#pragma region Com_VIBuffer
		//For.Com_VIBuffer
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
			L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
#pragma endregion
		return S_OK;
}

void CUserInterface::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	
	CGameObject::Free();
}
