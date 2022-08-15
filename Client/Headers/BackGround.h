#pragma once
#ifndef __BACKGROUND_H__

#include "GameObject.h"
BEGIN(Client)
class CBackGround : public CGameObject
{
public:
	enum SCENE_TEX { TEX_LOGO, TEX_FONT, TEX_MENU, TEX_END};

private:
	explicit CBackGround(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBackGround(const CBackGround& _other);
	virtual ~CBackGround() = default;

public:
	//Get
	const bool Get_NextScene() const;
	//Set
	void Set_SceneTexType(const SCENE_TEX _eTexType);

private:
	HRESULT Set_Transform(D3DXVECTOR3& _vPos, D3DXVECTOR3& _vSacle);

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	HRESULT	Add_Component();
	HRESULT Render_Font();

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	SCENE_TEX m_eSceneTexType = TEX_END;
	CTransform*	m_pTransformCom = nullptr;
	CVIBuffer_RectTexture* m_RectTextureCom = nullptr;
	CTexture* m_pTextureCom[TEX_END];

	int	  m_iCount = 0;
	float m_fx = 0.f;
	float m_fy = 0.f;
	float m_fSpeed = 10.f;

	bool m_IsNextScene = false;
};
END
#define __BACKGROUND_H__
#endif // !__USERINTERFACE_H__