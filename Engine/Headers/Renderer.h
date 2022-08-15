#pragma once

#ifndef __RENDERER_H__

#include "GameObject.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDER_GROUP 
	{ 
		RENDER_PRIORITY
		, RENDER_NONALPHA
		, RENDER_ALPHA
		, RENDER_UI
		, RENDER_END 
	};

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderer() = default;

public:
	HRESULT Add_RenderList(RENDER_GROUP eGroupID, CGameObject* pObject);
	HRESULT Set_OrthoMatrix(float _ScreenX, float _ScreenY);

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	typedef list<CGameObject*>	GAMEOBJECTS;
	GAMEOBJECTS	m_RenderList[RENDER_END];

	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	D3DXMATRIX			m_matOrthoProj;
};

END

#define __RENDERER_H__
#endif