#pragma once
#ifndef __FRAME_MGR_H__

#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CFrame_Manager : public CBase
{
public:
	explicit CFrame_Manager();
	virtual ~CFrame_Manager() = default;
public:
	void Init_Frame(float fFps);
	bool Unlock_Frame();
	void Render_Frame(HWND _hWnd);

	virtual void Free() override;
private:
	LARGE_INTEGER	m_tCpuTick;			// CPU Tick 
	LARGE_INTEGER	m_tBeginTime;		// Check Cpu Tick Time
	LARGE_INTEGER	m_tCurrentTime;			// Next Cpu Tick Time
	float			m_fDeltaTime;		// Delta Time
	float			m_fSecondPerFrame;	// Second per Frame
	float			m_fFPSTime;			// Fps Time
	int				m_iFPS;				// Frame Per Second
	TCHAR			m_szFPS[32];		// For Show Fps
};
END
#define __FRAME_MANAGER_H__
#endif // !__FRAME_MGR_H__
