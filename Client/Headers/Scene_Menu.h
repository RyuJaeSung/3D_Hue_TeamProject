#pragma once
#ifndef __SCENE_MENU_H__


#include "Scene.h"

BEGIN(Client)

class CLoading;
class CScene_Menu final : public CScene
{
private:
	explicit CScene_Menu(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene_Menu() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene(float fTimeDelta) override;
	virtual int LateUpdate_Scene(float fTimeDelta) override;

public:
	HRESULT Ready_Layer_Object();

public:
	static CScene_Menu* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	HWND		m_hVideo;
};

END
#define __SCENE_MENU_H__
#endif // !__SCENE_MENU_H__