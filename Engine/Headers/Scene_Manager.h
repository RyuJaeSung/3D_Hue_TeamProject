#pragma once

#ifndef __SCENE_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CScene;
class CScene_Manager final : public CBase
{
	DECLARE_SINGLETON(CScene_Manager)

private:
	explicit CScene_Manager();
	virtual ~CScene_Manager() = default;

public:
	// Getter
	CScene* Get_CurrentScene_Pointer();

public:
	HRESULT SetUp_CurrentScene(CScene* pCurrentScene);
	int Update_Scene(float fTimeDelta);
	int LateUpdate_Scene(float fTimeDelta);

public:
	virtual void Free() override;

private:
	CScene*	m_pCurrentScene = nullptr;
};

END

#define __SCENE_MANAGER_H__
#endif