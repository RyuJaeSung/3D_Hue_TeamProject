#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Scene_Menu.h"
#include "Loading.h"
#include "GameObject.h"
#include "BackGround.h"
USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CScene_Logo::Ready_Scene()
{
	m_pLoading = CLoading::Create(m_pDevice, SCENE_STAGE);
	if (nullptr == m_pLoading)
		return E_FAIL;

	if (FAILED(Ready_Layer_Object()))
		return E_FAIL;

	return S_OK;
}

int CScene_Logo::Update_Scene(float fTimeDelta)
{
	return 0;
}

int CScene_Logo::LateUpdate_Scene(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CBackGround* pBackGround = (CBackGround*)pManagement->Get_GameObject_Pointer(SCENE_LOGO, L"Layer_BackGround");


	if (m_pLoading->Get_IsFinished() &&
		pBackGround->Get_NextScene())
	{
		if (FAILED(pManagement->SetUp_CurrentScene(SCENE_MENU, CScene_Menu::Create(m_pDevice))))
			return 0;

		if (FAILED(pManagement->Clear_ForScene(SCENE_LOGO)))
			return 0;
	}

	
	return 0;
}

HRESULT CScene_Logo::Ready_Layer_Object()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_LOGO, L"GameObject_BackGround", SCENE_LOGO, L"Layer_BackGround")))
		return E_FAIL;
	CBackGround* pBackGround = (CBackGround*)pManagement->Get_GameObject_Pointer(SCENE_LOGO, L"Layer_BackGround");
	pBackGround->Set_SceneTexType(CBackGround::TEX_LOGO);
	return S_OK;
}


CScene_Logo* CScene_Logo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CScene_Logo* pInstance = new CScene_Logo(pDevice);
	
	if (FAILED(pInstance->Ready_Scene()))
	{
		ERR_MSG(L"Failed To Creating CScene_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	// 자식의 자원들 릴리즈라고 난 다음에
	Safe_Release(m_pLoading);

	// 부모 프리 반드시 호출
	CScene::Free();
}

