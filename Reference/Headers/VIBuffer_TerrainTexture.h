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
	// CVIBuffer��(��) ���� ��ӵ�
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
	DWORD m_dwVTXCntX = 0;	// ������ �����ϴ� ������ ���ؽ� ����.
	DWORD m_dwVTXCntZ = 0;	// ������ �����ϴ� ������ ���ؽ� ����.
	float m_fVTXItv = 0;	// ���ؽ� ����.
};

END

#define __VIBUFFER_TERRAINTEXTURE_H__
#endif