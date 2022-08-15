#pragma once

#ifndef __TILE_H__
#define __TILE_H__

BEGIN(Client)

class CTile : public CGameObject
{
protected:
	explicit CTile(LPDIRECT3DDEVICE9 pDevice);
	explicit CTile(const CTile& other);
	virtual ~CTile() = default;

public:
	// Getter

	// Setter
	void Set_TextureIndex(const int& _iTextureIdx) { m_iTextureIndex = _iTextureIdx; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

protected:
	virtual HRESULT Add_Component();
	HRESULT Collision_ToPlayer();

private:
	HRESULT Curling();
	HRESULT SetUp_CollisionDest(); // 타일의 위치 및 크기 지정

public:
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

protected:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCollision_Cube*		m_pCubeCollisionCom = nullptr;
	CFrustum*				m_pFrustumCom = nullptr;

	int						m_iTextureIndex = 0;

	bool					m_bInFrustum = true;
	bool					m_bIsinit = true;
};

END

#endif // !__TILE_H__


