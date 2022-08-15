#include "Frame_Manager.h"
#include "Management.h"

USING(Engine)

CFrame_Manager::CFrame_Manager()
{
	ZeroMemory(&m_tCpuTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tBeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tCurrentTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_szFPS, sizeof(m_szFPS));
	m_fDeltaTime = 0.f;
	m_fSecondPerFrame = 0.f;
	m_fFPSTime = 0.f;
	m_iFPS = 0;
}

void CFrame_Manager::Init_Frame(float fFps)
{
	m_fSecondPerFrame = 1.f / fFps;
	QueryPerformanceCounter(&m_tCurrentTime);		// 현재 성능 카운터 값
	QueryPerformanceFrequency(&m_tCpuTick);		// 현재 성능 카운터의 진동 수
	QueryPerformanceCounter(&m_tBeginTime);		// 시작 성능 카운터 값을 받음
}

bool CFrame_Manager::Unlock_Frame()
{
	QueryPerformanceCounter(&m_tCurrentTime);		// 시간 경과에 따라 현재 성능 카운터 값을 습득
	QueryPerformanceFrequency(&m_tCpuTick);			// 성능 카운터의 진동수가 변할 수 있으므로 경과마다 새로 받음
	m_fDeltaTime += float(m_tCurrentTime.QuadPart - m_tBeginTime.QuadPart) / m_tCpuTick.QuadPart;		// 경과 값을 받음
	m_tBeginTime = m_tCurrentTime;				// 시작 성능 카운터 값 변경
	if (m_fSecondPerFrame <= m_fDeltaTime)		// 경과 시간이 프레임 당 시간에 도달하면 경과 시간을 초기화하고 LockFrame을 해제
	{
		m_fDeltaTime = 0.f;
		return true;
	}
	return false;
}
void CFrame_Manager::Render_Frame(HWND _hWnd)
{
	++m_iFPS;
	m_fFPSTime += CManagement::Get_Instance()->Get_DeltaTime();
	if (1.f <= m_fFPSTime)
	{
		swprintf_s(m_szFPS, __T("FPS : %d"), m_iFPS);
		m_iFPS = 0;
		m_fFPSTime = 0.f;
	}
	SetWindowText(_hWnd, m_szFPS);
}
void CFrame_Manager::Free()
{
}

