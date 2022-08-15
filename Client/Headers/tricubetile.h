#pragma once

#ifndef __TRICUBETILE_H__
#define __TRICUBETILE_H__

#include "Tile.h"

BEGIN(Client)

class CTriCubeTile : public CTile
{
private:
	explicit CTriCubeTile(LPDIRECT3DDEVICE9 pDevice);
	explicit CTriCubeTile(const CTriCubeTile& other);
	virtual ~CTriCubeTile() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

protected:
	virtual HRESULT Add_Component();

public:
	static CTriCubeTile* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_TriCubeTexture*		m_pVIBufferCom = nullptr;
};

END

#endif // !__CUBETILE_H__



