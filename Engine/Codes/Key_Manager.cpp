#include "..\Headers\Key_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CKey_Manager)
CKey_Manager::CKey_Manager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

bool CKey_Manager::KeyDown(int _iKey)
{
	if (!m_bKeyState[_iKey] && GetAsyncKeyState(_iKey) & 0x8000)
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}
	return false;
}

bool CKey_Manager::KeyUp(int _iKey)
{
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}
	return false;
}

bool CKey_Manager::KeyPressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;
	return false;
}

void CKey_Manager::KeyUpdate()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}

void CKey_Manager::Free()
{
}
