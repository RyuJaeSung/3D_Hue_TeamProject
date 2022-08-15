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
	QueryPerformanceCounter(&m_tCurrentTime);		// ���� ���� ī���� ��
	QueryPerformanceFrequency(&m_tCpuTick);		// ���� ���� ī������ ���� ��
	QueryPerformanceCounter(&m_tBeginTime);		// ���� ���� ī���� ���� ����
}

bool CFrame_Manager::Unlock_Frame()
{
	QueryPerformanceCounter(&m_tCurrentTime);		// �ð� ����� ���� ���� ���� ī���� ���� ����
	QueryPerformanceFrequency(&m_tCpuTick);			// ���� ī������ �������� ���� �� �����Ƿ� ������� ���� ����
	m_fDeltaTime += float(m_tCurrentTime.QuadPart - m_tBeginTime.QuadPart) / m_tCpuTick.QuadPart;		// ��� ���� ����
	m_tBeginTime = m_tCurrentTime;				// ���� ���� ī���� �� ����
	if (m_fSecondPerFrame <= m_fDeltaTime)		// ��� �ð��� ������ �� �ð��� �����ϸ� ��� �ð��� �ʱ�ȭ�ϰ� LockFrame�� ����
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

