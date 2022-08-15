#pragma once

#ifndef __TIME_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CTime_Manager final : public CBase
{
	DECLARE_SINGLETON(CTime_Manager)

private:
	explicit CTime_Manager();
	virtual ~CTime_Manager() = default;

public:
	HRESULT Ready_Time_Manager(); 
	float Update_Time();
	float Get_DeltaTime() { return m_fDeltaTime; }

public:
	virtual void Free() override;

private:
	LARGE_INTEGER m_CpuTick; 
	LARGE_INTEGER m_OldTime_First; 
	LARGE_INTEGER m_OldTime_Second; 
	LARGE_INTEGER m_CurTime;

	float		m_fDeltaTime; 

};

END

#define __TIME_MANAGER_H__
#endif

