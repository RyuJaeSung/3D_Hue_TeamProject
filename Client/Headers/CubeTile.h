#pragma once

#ifndef __CUBETILE_H__
#define __CUBETILE_H__

#include "Tile.h"

BEGIN(Client)

class CCubeTile : public CTile
{
private:
	explicit CCubeTile(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeTile(const CCubeTile& other);
	virtual ~CCubeTile() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

protected:
	virtual HRESULT Add_Component();

public:
	static CCubeTile* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_CubeTexture*		m_pVIBufferCom = nullptr;

};

END

#endif // !__CUBETILE_H__



