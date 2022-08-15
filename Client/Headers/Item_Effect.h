#pragma once

#ifndef __ITEM_EFFECT_H__

#include "GameObject.h"

BEGIN(Client)

class CItem_Effect final : public CGameObject
{
private:
	explicit CItem_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Effect(const CItem_Effect& other);
	virtual ~CItem_Effect() = default;

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
	static CItem_Effect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	int						m_iIndex = 0;
	D3DXVECTOR3				m_vDir;

};

END


#define __STONE_EFFECT_H__
#endif // !__STONE_EFFCT_H__


