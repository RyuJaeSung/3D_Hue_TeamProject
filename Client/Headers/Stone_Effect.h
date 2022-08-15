#pragma once
#include "GameObject.h"

BEGIN(Client)

class CStone_Effect final : public CGameObject
{
public: // 플레이어가 Right->돌은 left
	enum DIR {RIGHT, LEFT};
private:
	explicit CStone_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CStone_Effect(const CStone_Effect& other);
	virtual ~CStone_Effect() = default;

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
	static CStone_Effect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	float					m_fAngle;
	DIR						m_eDir;

};

END

#define __STONE_EFFECT_H__



