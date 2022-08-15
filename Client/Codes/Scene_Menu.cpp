#include "stdafx.h"
#include "..\Headers\Scene_Menu.h"
#include "Scene_Stage.h"

#include <Vfw.h>
#pragma comment (lib, "Vfw32.lib")

USING(Client)

CScene_Menu::CScene_Menu(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CScene_Menu::Ready_Scene()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

#ifndef _DEBUG
	
	m_hVideo = MCIWndCreate(g_hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR
		, L"../Resources/Video/Menu.wmv");
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, NULL);

	MCIWndPlay(m_hVideo);

#endif // !_DEBUG
	return S_OK;
}

int CScene_Menu::Update_Scene(float fTimeDelta)
{
	return 0;
}

int CScene_Menu::LateUpdate_Scene(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (pManagement->KeyDown(VK_SPACE))
	{
		if (FAILED(pManagement->SetUp_CurrentScene(SCENE_STAGE1, CScene_Stage::Create(m_pDevice, SCENE_STAGE1))))
			return 0;

#ifndef _DEBUG
		MCIWndStop(m_hVideo);
#endif // !_DEBUG

		if (FAILED(pManagement->Clear_ForScene(SCENE_MENU)))
			return 0;
	}

	return 0;
}

HRESULT CScene_Menu::Ready_Layer_Object()
{
	return S_OK;
}

CScene_Menu * CScene_Menu::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScene_Menu* pInstance = new CScene_Menu(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		ERR_MSG(L"Failed To Creating CScene_Menu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Menu::Free()
{	
	CScene::Free();
}
