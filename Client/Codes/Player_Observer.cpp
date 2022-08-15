#include "stdafx.h"
#include "Player_Observer.h"

USING(Client)
CPlayer_Observer::CPlayer_Observer()
{
}

void CPlayer_Observer::Update_Observer(int iMessage, void * pData)
{
	switch (iMessage)
	{
	case OBJ_DATA::PLAYER_COLOR:
		m_dwColor = *static_cast<DWORD*>(pData);
		break;
	case OBJ_DATA::PLAYER_POSITION:
		m_vPos = *static_cast<D3DXVECTOR3*>(pData);
		break;
	}
}

