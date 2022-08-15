#pragma once

#ifndef __SCENE_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
public:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

public:
	virtual HRESULT Ready_Scene() = 0;
	virtual int Update_Scene(float fTimeDelta) = 0;
	virtual int LateUpdate_Scene(float fTimeDelta) = 0;

public:
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
};

END

#define __SCENE_H__
#endif
