#pragma once

#ifndef __SCENE_LOGO_H__

#include "Scene.h"

BEGIN(Client)

class CLoading;
class CScene_Logo final : public CScene
{
public:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene_Logo() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene(float fTimeDelta) override;
	virtual int LateUpdate_Scene(float fTimeDelta) override;

public:
	HRESULT Ready_Layer_Object();

public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	CLoading*	m_pLoading = nullptr;
};

END

#define __SCENE_LOGO_H__
#endif
