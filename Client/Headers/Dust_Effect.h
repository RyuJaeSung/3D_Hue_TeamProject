#pragma once
#ifndef __DUST_EFFECT_H__
#include "GameObject.h"

BEGIN(Client)

class CDust_Effect final : public CGameObject
{
private:
	explicit CDust_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CDust_Effect(const CDust_Effect& other);
	virtual ~CDust_Effect() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float fTimeDelta);

public:
	static CDust_Effect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	int						m_iIndex = 0;
};

END


#define __STONE_EFFECT_H__
#endif // !__STONE_EFFCT_H__


