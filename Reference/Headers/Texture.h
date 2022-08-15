#pragma once

#ifndef __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURE_TYPE
	{
		TEXTURE_NORMAL,
		TEXTURE_CUBE
	};

public:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& other);
	virtual ~CTexture() = default;

public:	
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;

public:
	HRESULT Ready_Component_Prototype(TEXTURE_TYPE eType, const wstring& FilePath, int iCount);
	HRESULT Set_Texture(int iIndex);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, TEXTURE_TYPE eType, const wstring& FilePath, int iCount);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;

private:
	//LPDIRECT3DTEXTURE9
	//LPDIRECT3DCUBETEXTURE9
	typedef vector<IDirect3DBaseTexture9*>	TEXTURES;
	TEXTURES m_Textures;

};

END

#define __TEXTURE_H__
#endif