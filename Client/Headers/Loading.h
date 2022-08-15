#pragma once

#ifndef __LOADING_H__

#include "Base.h"

BEGIN(Client)

class CLoading final : public CBase
{
public:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice, SCENE_ID eNextSceneID);
	virtual ~CLoading() = default;

public:
	// Getter
	bool Get_IsFinished() const;

public:
	HRESULT Ready_Loading();

private:
	HRESULT Ready_Stage_Resources();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, SCENE_ID eNextSceneID);
	virtual void Free() override;

public: // 쓰레드 함수
	static unsigned __stdcall ThreadMain(void* pParam);

private:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	SCENE_ID			m_eNextSceneID = SCENE_END;

	HANDLE				m_hLoadingThread = nullptr;
	CRITICAL_SECTION	m_CriticalSection;

	bool				m_IsFinished = false;
};

END

#define __LOADING_H__
#endif