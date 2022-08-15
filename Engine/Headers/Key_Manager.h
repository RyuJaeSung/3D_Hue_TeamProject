#pragma once
#ifndef __KEY_MANAGER_H__

#include "Base.h"
BEGIN(Engine)
#define VK_MAX 0xff
class CKey_Manager final : public CBase	
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	explicit CKey_Manager();
	virtual ~CKey_Manager() = default;
public:
	bool KeyDown(int _iKey);
	bool KeyUp(int _iKey);
	bool KeyPressing(int _iKey);
public:
	void KeyUpdate();
public:
	virtual void Free() override;
private:
	bool m_bKeyState[VK_MAX];
};
END
#define __KEY_MANAGER_H__
#endif // !__KEY_MANAGER_H__