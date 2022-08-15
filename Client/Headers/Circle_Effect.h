#pragma once
#ifndef __CIRCLE_EFFECT_H__

#include "GameObject.h"
#include "Observer.h"
class CCircle_Effect final : public CGameObject
{
private:
	explicit CCircle_Effect(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCircle_Effect(const CCircle_Effect& _other);
	virtual ~CCircle_Effect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	// Setter
	void Click(bool _bClick) { m_bClick = _bClick; }

private:
	void Set_Effect_Size();
	void Set_Player_Pos();
	HRESULT Render_Color_Object();
	HRESULT Add_Component();

public:
	static CCircle_Effect* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free();

private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	CObserver*				m_pPlayer_Observer = nullptr;

	
	D3DXMATRIX				m_matWorld;
	D3DXVECTOR3				m_vEffectScale;
	D3DXVECTOR3				m_vPlayerPos;

	bool					m_bClick = false;		// 클릭되어 UI출력될 때 이펙트도 진행되어야 함
	bool					m_bShow = false;		// 크기가 남아있는 동안은 계속 보여줘야함
	float					m_fEffectSpeed = 0.f;	// 크기 변환 속도
	float					m_fEndSize = 0.f;
};

#define __CIRCLE_EFFECT_H__
#endif // !__CIRCLE_EFFECT_H__