#pragma once

#ifndef __RENDERER_H__

#include "GameObject.h"

BEGIN(Engine)

// 우리 게임의 렌더링을 그룹화하여 담당한다.
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
	HRESULT Set_OrthoMatrix(float _ScreenX, float _ScreenY);		// 직교 투영 행렬을 세팅하기 위한 화면의 너비, 높이를 입력받음.
	HRESULT Render();

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