#pragma once

#ifndef __ITEM_H__

#include "MyObject.h"

BEGIN(Client)

class CItem : public CMyObject
{
public:
	typedef struct tagItem_Desc
	{
		// 정보 받아오는 용도
		D3DXVECTOR3			vItemPos = {0.f, 0.f, 0.f};
		COLOR_TYPE			eColorType = COLOR_END;
	}ITEM_DESC;

private:
	enum TEXTURE_TYPE
	{
		TEXTURE_BG,
		TEXTURE_GLASS,
		TEXTURE_END
	};

private:
	explicit CItem(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem(const CItem& other);
	virtual ~CItem() = default;

public:
	// CMyObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer(float _fTimeDelta);
	HRESULT Create_Item_Effect();
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc);

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_RectTexture*	m_pItemVIBufferCom[TEXTURE_END];
	CTexture*			m_pItemTextureCom[TEXTURE_END];
	bool				m_bInit = true;
	ITEM_DESC			m_Item_Desc;
	int					m_iRanderState = 0;
};

END

#define __ITEM_H__
#endif