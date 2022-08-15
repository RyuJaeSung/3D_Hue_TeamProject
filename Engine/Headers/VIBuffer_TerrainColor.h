#pragma once

#ifndef __VIBUFFER_TERRAINCOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TerrainColor final : public CVIBuffer
{
public:
	explicit CVIBuffer_TerrainColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_TerrainColor(const CVIBuffer_TerrainColor& other);
	virtual ~CVIBuffer_TerrainColor() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_TerrainColor* Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwVTXCntX, DWORD dwVTXCntZ, float fVTXItv = 1.f);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;

private:
	DWORD m_dwVTXCntX = 0;	
	DWORD m_dwVTXCntZ = 0;
	float m_fVTXItv = 0;
};

END

#define __VIBUFFER_RECTCOLOR_H__
#endif