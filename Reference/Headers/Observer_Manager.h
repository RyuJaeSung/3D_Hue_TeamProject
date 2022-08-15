#pragma once
#ifndef __OBSERVER_MANAGER_H__
#include "Base.h"
BEGIN(Engine)
class CObserver;
class ENGINE_DLL CObserver_Manager :
	public CBase
{
DECLARE_SINGLETON(CObserver_Manager)
private:
	explicit CObserver_Manager();
	virtual ~CObserver_Manager() = default;
public:
	void Subscribe(CObserver* _pObserver);
	void Unsubscribe(CObserver* _pObserver);
	void Notify(int _iMessage, void* _pData);
public:
	virtual void Free() override;
private:
	list<CObserver*> m_listObserver;
};

END
#define __OBSERVER_MANAGER_H__
#endif // !__OBSERVER_MANAGER_H__