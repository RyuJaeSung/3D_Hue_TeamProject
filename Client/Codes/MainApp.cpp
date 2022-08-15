#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Scene_Logo.h"
#include "Player.h"
#include "Player_Hand.h"
#include "Static_Camera.h"
#include "Palette_Circle.h"
#include "Quest_Window.h"
#include "Item_Inven.h"
#include "BackGround.h"

USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	Safe_AddRef(m_pManagement);

	ShowCursor(FALSE);
}


CMainApp::~CMainApp()
{
}

int CMainApp::Update_MainApp()
{
	if (nullptr == m_pManagement)
		return 0;

	m_pManagement->Update_Engine();
	
	return 0;
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	m_pManagement->Render_Engine();

	return S_OK;
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(m_pManagement->Ready_Engine(SCENE_END, g_hWnd, WINCX, WINCY, CGraphic_Device::WINMODE_WIN)))
		return E_FAIL;

	m_pDevice = m_pManagement->Get_Device();
	if (nullptr == m_pDevice)
		return E_FAIL;

	Safe_AddRef(m_pDevice);

	if (FAILED(Ready_Static_Resources()))
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(SCENE_LOGO, CScene_Logo::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Resources()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.GameObject------------------------------------------------------------------------------------
#pragma region GameObject_Player
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Player_Hand", CPlayer_Hand::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Static_Camera
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Static_Camera", CStatic_Camera::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Palette
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Palette", CPalette_Circle::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_CircleEffect", CCircle_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Quest_Window
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Quest_Window", CQuest_Window::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item_Inven
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Item_Inven", CItem_Inven::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// For.Component-------------------------------------------------------------------------------------
#pragma region Component_VIBuffer_RectColor	
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_RectColor", CVIBuffer_RectColor::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_RectTexture
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_RectTexture", CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_CubeTexture
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", CVIBuffer_CubeTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_CubeTexture
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_TriCubeTexture", CVIBuffer_TriCubeTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Transform	
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

// For.Logo-------------------------------------------------------------------------------------
#pragma region Component_Texture_Logo_Font
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_LOGO, L"Component_Logo_Font", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Scene/Logo/Font/Font%d.png", 7))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture__Logo_BackGround
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_LOGO, L"Component_Logo_BackGround", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Scene/Logo/BackGround/Logo%d.png", 1))))
		return E_FAIL;
#pragma endregion


#pragma region GameObject_BackGround
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_LOGO, L"GameObject_BackGround", CBackGround::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	// Α¶Έν off.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		ERR_MSG(L"Failed To Creating MainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pManagement);	
	Safe_Release(m_pDevice);

	CManagement::Release_Engine();
}
