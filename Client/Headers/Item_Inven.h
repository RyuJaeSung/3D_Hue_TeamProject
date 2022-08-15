#pragma once

#ifndef __ITEM_INVEN_H__
#define __ITEM_INVEN_H__

#include "UserInterface.h"

BEGIN(Client)

class CItem_Inven final : public CUserInterface
{
public:
	enum ITEM_COLOR
	{
		ITEM_RED,
		ITEM_ORANGE,
		ITEM_YELLOW,
		ITEM_GREEN,
		ITEM_BLUE,
		ITEM_NAVY,
		ITEM_PURPLE,
		ITEM_PINK,
		ITEM_END
	};

private:
	enum TEXTURE_TYPE
	{
		TEXTURE_BG,
		TEXTURE_GLASS,
		TEXTURE_END
	};

private:
	explicit CItem_Inven(LPDIRECT3DDEVICE9 _pDevice);
	explicit CItem_Inven(const CItem_Inven& _other);
	virtual ~CItem_Inven() = default;

public:
	// Getter
	bool* Get_ItemState() { return m_bGetItem; }

	// Setter
	void Set_ItemState_True(const ITEM_COLOR& _eItemColor) { m_bGetItem[_eItemColor] = true; }

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	DWORD SetUp_ItemColor(const ITEM_COLOR& _eItemColor);

public:
	static CItem_Inven* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_RectTexture*	m_pItemVIBufferCom[TEXTURE_END];
	CTexture*				m_pItemTextureCom[TEXTURE_END];

	bool					m_bGetItem[ITEM_END]; // æ∆¿Ã≈€¿ª »πµÊ «ﬂ¥¬∞°?
};

END

#endif // !__ITEM_INVEN_H__


