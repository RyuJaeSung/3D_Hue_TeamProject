#include "..\Headers\Object_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CGameObject * CObject_Manager::Get_GameObject_Pointer(int iSceneID, const wstring & LayerTag, int iIndex)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return nullptr;

	if (nullptr == m_pLayers)
		return nullptr;

	auto iter_find = m_pLayers[iSceneID].find(LayerTag);
	if (m_pLayers[iSceneID].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_GameObject_Pointer(iIndex);
}

const list<CGameObject*>* CObject_Manager::Get_GameObject_List(int iSceneID, const wstring & LayerTag)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return nullptr;

	if (nullptr == m_pLayers)
		return nullptr;

	auto iter_find = m_pLayers[iSceneID].find(LayerTag);
	if (m_pLayers[iSceneID].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_GameObject_List();
}

CComponent* CObject_Manager::Get_Component_Pointer(int iSceneID, const wstring & LayerTag, const wstring & ComponentTag, int iObjectIndex)
{
	CGameObject* pGameObject = Get_GameObject_Pointer(iSceneID, LayerTag, iObjectIndex);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject->Find_Component(ComponentTag);
}

HRESULT CObject_Manager::Reserve_Container(int iSceneCount)
{
	if (1 > iSceneCount)
		return E_FAIL;

	if (nullptr == m_pPrototypes)
		m_pPrototypes = new PROTOTYPES[iSceneCount];
	if (nullptr == m_pLayers)
		m_pLayers = new LAYERS[iSceneCount];

	m_iSceneCount = iSceneCount;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_Prototype(int iSceneID, const wstring & GameObjectTag, CGameObject* pProtoType)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return E_FAIL;

	if (nullptr == m_pPrototypes)
		return E_FAIL;

	auto iter_find = m_pPrototypes[iSceneID].find(GameObjectTag);

	if (m_pPrototypes[iSceneID].end() == iter_find)
		m_pPrototypes[iSceneID][GameObjectTag] = pProtoType;
	else
		Safe_Release(pProtoType);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(int iSceneID_FromPrototype, const wstring& GameObjectTag, int iSceneID_ToLayer, const wstring& LayerTag, void* pArg)
{
	if (0 > iSceneID_FromPrototype ||
		m_iSceneCount <= iSceneID_FromPrototype)
		return E_FAIL;

	if (0 > iSceneID_ToLayer ||
		m_iSceneCount <= iSceneID_ToLayer)
		return E_FAIL;

	if (nullptr == m_pPrototypes ||
		nullptr == m_pLayers)
		return E_FAIL;

	PROTOTYPES::iterator iter_find_Prototype = m_pPrototypes[iSceneID_FromPrototype].find(GameObjectTag);
	if (m_pPrototypes[iSceneID_FromPrototype].end() == iter_find_Prototype)
		return E_FAIL;

	LAYERS::iterator iter_find_Layer = m_pLayers[iSceneID_ToLayer].find(LayerTag);
	if (m_pLayers[iSceneID_ToLayer].end() == iter_find_Layer)
		m_pLayers[iSceneID_ToLayer].insert(make_pair(LayerTag, CLayer::Create()));

	CGameObject* pClone = iter_find_Prototype->second->Clone_GameObject(pArg);
	if (nullptr == pClone)
		return E_FAIL;

	if (FAILED(m_pLayers[iSceneID_ToLayer][LayerTag]->Add_GameObject_ToLayer(pClone)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Manager::Clear_ForScene(int iSceneID)
{
	if (0 > iSceneID ||
		m_iSceneCount <= iSceneID)
		return E_FAIL;		

	// 해당 씬의 Layer 정리
	if(nullptr == m_pLayers)
		return E_FAIL;

	for (auto& MyPair : m_pLayers[iSceneID])
	{
		MyPair.second->Clear_Layer();
		Safe_Release(MyPair.second);
	}

	m_pLayers[iSceneID].clear();	

	// 해당 씬의 원형 객체 정리
	if (nullptr == m_pPrototypes)
		return E_FAIL;

	for (auto& MyPair : m_pPrototypes[iSceneID])
		Safe_Release(MyPair.second);

	m_pPrototypes[iSceneID].clear();

	return S_OK;
}

int CObject_Manager::Update_Object_Manager(float fTimeDelta)
{
	for (int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& MyPair : m_pLayers[i])
			MyPair.second->Update_Layer(fTimeDelta);
	}

	return 0;
}

int CObject_Manager::LateUpdate_Object_Manager(float fTimeDelta)
{
	for (int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& MyPair : m_pLayers[i])
			MyPair.second->LateUpdate_Layer(fTimeDelta);
	}

	return 0;
}

void CObject_Manager::Free()
{
	for (int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& MyPair : m_pLayers[i])
			Safe_Release(MyPair.second);

		for (auto& MyPair : m_pPrototypes[i])
			Safe_Release(MyPair.second);

		m_pLayers[i].clear();
		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pLayers);
	Safe_Delete_Array(m_pPrototypes);
}
