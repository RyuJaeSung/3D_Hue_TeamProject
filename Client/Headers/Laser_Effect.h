#pragma once

#ifndef __LASER_EFFECT_H__

BEGIN(Client)

class CLaser_Effect final : public CGameObject
{
private:
	explicit CLaser_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CLaser_Effect(const CLaser_Effect& other);
	virtual ~CLaser_Effect() = default;

public:
	// Getter

	// Setter
	void Set_Dir(const D3DXVECTOR3& _vDir) { m_vDir = _vDir; }
	void Set_TextureColor(const COLOR_TYPE& _eColorType);

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
	void SetUp_TextureColor();

public:
	static CLaser_Effect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	CObserver*				m_pObserver = nullptr;

	int						m_iIndex = 0;
	D3DXVECTOR3				m_vDir = { 0.f, 0.f, 0.f };

	COLOR_TYPE				m_eColorType = COLOR_END;
	DWORD					m_dwColor = 0;
};

END


#define __LASER_EFFECT_H__
#endif // !__LASER_EFFECT_H__


