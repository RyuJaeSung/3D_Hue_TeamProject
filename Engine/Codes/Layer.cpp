#include "..\Headers\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}


CGameObject* CLayer::Get_GameObject_Pointer(int iIndex)
{
	if (0 > iIndex ||
		m_GameObjects.size() <= (size_t)iIndex)
		return nullptr;

	if (m_GameObjects.empty())
		return nullptr;

	GAMEOBJECTS::iterator iter_begin = m_GameObjects.begin();

	for (int i = 0; i < iIndex; ++i)
		++iter_begin;

	return *iter_begin;
}

const list<CGameObject*>* CLayer::Get_GameObject_List()
{
	return &m_GameObjects;
}

HRESULT CLayer::Add_GameObject_ToLayer(CGameObject* pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	auto iter_find = find(m_GameObjects.begin(), m_GameObjects.end(), pObject);
	if (m_GameObjects.end() != iter_find)
		return E_FAIL;

	m_GameObjects.push_back(pObject);

	return S_OK;
}

int CLayer::Update_Layer(float fTimeDelta)
{
	Clear_NoneAlived_Object();

	EVENT eDead = EVENT_END;
	for (auto& pObject : m_GameObjects)
	{
		eDead = (EVENT)pObject->Update_GameObject(fTimeDelta);
		if (eDead == DEAD)
			pObject->Dead_Ailved();
	}

	return 0;
}

int CLayer::LateUpdate_Layer(float fTimeDelta)
{
	for (auto& pObject : m_GameObjects)
		pObject->LateUpdate_GameObject(fTimeDelta);

	return 0;
}

HRESULT CLayer::Clear_Layer()
{
	for (auto& pObject : m_GameObjects)
		Safe_Release(pObject);

	m_GameObjects.clear();

	return S_OK;
}

void CLayer::Clear_NoneAlived_Object()
{
	GAMEOBJECTS::iterator iter_begin = m_GameObjects.begin();
	GAMEOBJECTS::iterator iter_end = m_GameObjects.end();

	for (; iter_begin != iter_end; )
	{
		if (false == (*iter_begin)->Is_Alived())
		{
			Safe_Release(*iter_begin);
			iter_begin = m_GameObjects.erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pObject : m_GameObjects)
		Safe_Release(pObject);

	m_GameObjects.clear();
}
