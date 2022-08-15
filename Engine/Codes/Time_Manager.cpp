#include "Time_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{
}

HRESULT CTime_Manager::Ready_Time_Manager()
{
	if (!QueryPerformanceFrequency(&m_CpuTick))
		return E_FAIL;

	if (!QueryPerformanceCounter(&m_OldTime_First))
		return E_FAIL;

	if (!QueryPerformanceCounter(&m_OldTime_Second))
		return E_FAIL;

	if (!QueryPerformanceCounter(&m_CurTime))
		return E_FAIL;

	return S_OK;
}

float CTime_Manager::Update_Time()
{	
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime_Second.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_OldTime_Second = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime_First.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime_First = m_CurTime;

	return m_fDeltaTime;
}

void CTime_Manager::Free()
{
}
