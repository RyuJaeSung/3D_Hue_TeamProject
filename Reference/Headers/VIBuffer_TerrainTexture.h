#pragma once

#ifndef __VIBUFFER_TERRAINTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TerrainTexture final : public CVIBuffer
{
public:
	explicit CVIBuffer_TerrainTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_TerrainTexture(const CVIBuffer_TerrainTexture& other);
	virtual ~CVIBuffer_TerrainTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg) override;
	virtual HRESULT Render_VIBuffer() override;

private:
	DWORD* LoadHeightMap_FromFile(const wstring& FilePath);

public:
	static CVIBuffer_TerrainTexture* Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwVTXCntX, DWORD dwVTXCntZ, float fVTXItv = 1.f);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;

private:
	DWORD m_dwVTXCntX = 0;	// 지형을 구성하는 가로의 버텍스 개수.
	DWORD m_dwVTXCntZ = 0;	// 지형을 구성하는 세로의 버텍스 개수.
	float m_fVTXItv = 0;	// 버텍스 간격.
};

END

#define __VIBUFFER_TERRAINTEXTURE_H__
#endif