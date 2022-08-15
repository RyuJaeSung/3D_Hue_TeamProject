#pragma once
#ifndef __KEY_MANAGER_H__

#include "Base.h"
BEGIN(Engine)
// 우재썜 방식으로 하는 편이 엔진 구성에 효율적
// 지정 키맵핑을 하고 싶으면 이 코드를 빼고 병건쌤 방식을 채택하면 됨
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