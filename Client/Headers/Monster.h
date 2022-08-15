#pragma once

#ifndef __MONSTER_H__

#include "GameObject.h"

BEGIN(Client)

class CMonster final : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Collision_ToTerrain();

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_CubeTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	CCollision_Terrain*		m_pCollisionCom = nullptr;
	CCollision_Cube*		m_pCubeCollisionCom = nullptr;
	CCollision_Sphere*		m_pSphereCollisionCom = nullptr;

	//Y축에 위에 있는 상자의 주소값
	CGameObject*			m_pYBox;
	//Y축에 넘겨줄 힘
	D3DXVECTOR3				m_vPower;
};

END

#define __MONSTER_H__
#endif