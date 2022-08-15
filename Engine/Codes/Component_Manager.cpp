#include "..\Headers\Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Container(int iSceneCount)
{
	if (1 > iSceneCount)
		return E_FAIL;

	if (nullptr == m_pPrototypes)
	{
		m_pPrototypes = new PROTOTYPES[iSceneCount];
		m_iSceneCount = iSceneCount;
	}

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(int iSceneID, const wstring& ComponentTag, CComponent* pProtoType)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return E_FAIL;

	if (nullptr == m_pPrototypes)
		return E_FAIL;

	auto iter_find = m_pPrototypes[iSceneID].find(ComponentTag);

	if (m_pPrototypes[iSceneID].end() == iter_find)
		m_pPrototypes[iSceneID][ComponentTag] = pProtoType;
	else
		Safe_Release(pProtoType);

	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(int iSceneID, const wstring& ComponentTag, void* pArg)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return nullptr;

	if (nullptr == m_pPrototypes)
		return nullptr;

	auto iter_find = m_pPrototypes[iSceneID].find(ComponentTag);
	if (m_pPrototypes[iSceneID].end() == iter_find)
		return nullptr;

	return iter_find->second->Clone_Component(pArg);
}

HRESULT CComponent_Manager::Clear_ForScene(int iSceneID)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return E_FAIL;

	if (nullptr == m_pPrototypes)
		return E_FAIL;

	for (auto& MyPair : m_pPrototypes[iSceneID])
		Safe_Release(MyPair.second);

	m_pPrototypes[iSceneID].clear();

	return S_OK;
}

void CComponent_Manager::Free()
{
	for (int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& MyPair : m_pPrototypes[i])
			Safe_Release(MyPair.second);

		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
}
