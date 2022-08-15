#pragma once
#ifndef __PLAYER_OBSERVER_H__

#include "Observer.h"

BEGIN(Client)
class CPlayer_Observer final : public Engine::CObserver
{
public:
	explicit CPlayer_Observer();
	virtual ~CPlayer_Observer() = default;

public:
	DWORD Get_Color() { return m_dwColor; }
	D3DXVECTOR3 Get_Pos() { return m_vPos; }
	
public:
	// CObserver을(를) 통해 상속됨
	virtual void Update_Observer(int iMessage, void * pData) override;

private:
	DWORD m_dwColor;
	D3DXVECTOR3 m_vPos;
};
END

#define __PLAYER_OBSERVER_H__
#endif // !__PLAYER_OBSERVER_H__