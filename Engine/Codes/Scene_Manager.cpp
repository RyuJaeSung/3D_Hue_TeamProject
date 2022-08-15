#include "..\Headers\Scene_Manager.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
{
}

CScene * CScene_Manager::Get_CurrentScene_Pointer()
{
	return m_pCurrentScene;
}

HRESULT CScene_Manager::SetUp_CurrentScene(CScene* pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (nullptr != m_pCurrentScene)
		Safe_Release(m_pCurrentScene);

	m_pCurrentScene = pCurrentScene;

	return S_OK;
}

int CScene_Manager::Update_Scene(float fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->Update_Scene(fTimeDelta);
}

int CScene_Manager::LateUpdate_Scene(float fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->LateUpdate_Scene(fTimeDelta);
}

void CScene_Manager::Free()
{
	Safe_Release(m_pCurrentScene);
}
