#pragma once
#ifndef __USERINTERFACE_H__

#include "GameObject.h"
BEGIN(Client)
class CUserInterface abstract : public CGameObject
{
protected:
	explicit CUserInterface(LPDIRECT3DDEVICE9 _pDevice);
	explicit CUserInterface(const CUserInterface& _other);
	virtual ~CUserInterface() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	virtual HRESULT Add_Component();

public:
	virtual CGameObject * Clone_GameObject(void * pArg)PURE;
	virtual void Free() override;
protected:
	CVIBuffer_RectTexture*	m_pVIBufferCom;
	CTexture*				m_pTextureCom;
	D3DXVECTOR3				m_vScale;
	D3DXVECTOR3				m_vPos;
	D3DXMATRIX				m_matWorld;
};
END
#define __USERINTERFACE_H__
#endif // !__USERINTERFACE_H__