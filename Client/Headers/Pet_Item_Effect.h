#pragma once

#ifndef __PET_ITEM_EFFECT_H__

#include "GameObject.h"

BEGIN(Client)

class CPet_Item_Effect final : public CGameObject
{
public:
	enum ITEM_EFFECT_STATUS { ITEM_EFFECT_PLAY, ITEM_EFFECT_STOP, ITEM_EFFECT_DEAD, ITEM_EFFECT_END };

public:
	// Setter
	void Set_Item_Effect_Status(ITEM_EFFECT_STATUS _eStatus) { m_eItemStatus = _eStatus; }

private:
	explicit CPet_Item_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CPet_Item_Effect(const CPet_Item_Effect& other);
	virtual ~CPet_Item_Effect() = default;

public:
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
	static CPet_Item_Effect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	int						m_iIndex = 0;
	D3DXVECTOR3				m_vDir;
	bool					m_bReverse = false;
	ITEM_EFFECT_STATUS		m_eItemStatus = ITEM_EFFECT_END;
};

END

#define __PET_ITEM_EFFECT_H__
#endif // !__PET_ITEM_EFFECT_H__