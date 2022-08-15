#pragma once
#ifndef __OBSERVER_H__

#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CObserver abstract : public CBase
{
protected:
	explicit CObserver();
	virtual ~CObserver() = default;
public:
	virtual void Update_Observer(int iMessage, void* pData)PURE;
	virtual void Free() override;
};
END
#define __OBSERVER_H__
#endif // !__OBSERVER_H__