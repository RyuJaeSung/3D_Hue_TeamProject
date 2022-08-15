#include "..\Headers\Observer_Manager.h"
#include "Observer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CObserver_Manager)
CObserver_Manager::CObserver_Manager()
{
}

void CObserver_Manager::Subscribe(CObserver * _pObserver)
{
	m_listObserver.emplace_back(_pObserver);
	Safe_AddRef(_pObserver);
}

void CObserver_Manager::Unsubscribe(CObserver * _pObserver)
{
	Safe_Release(_pObserver);
	auto& iter_find = find(m_listObserver.begin(), m_listObserver.end(), _pObserver);
	if (iter_find == m_listObserver.end())
		return;
	m_listObserver.erase(iter_find);
}

void CObserver_Manager::Notify(int _iMessage, void * _pData)
{
	for (auto& pObserver : m_listObserver)
		pObserver->Update_Observer(_iMessage, _pData);
}

void CObserver_Manager::Free()
{
	for_each(m_listObserver.begin(), m_listObserver.end(), Safe_Release<CObserver*>);
		
	m_listObserver.clear();
}
