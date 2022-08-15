#pragma once

#ifndef __TERRAIN_H__

#include "GameObject.h"

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& other);
	virtual ~CTerrain() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_TerrainTexture*		m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
};

END

#define __TERRAIN_H__
#endif