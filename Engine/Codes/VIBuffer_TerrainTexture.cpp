#include "..\Headers\VIBuffer_TerrainTexture.h"

USING(Engine)

CVIBuffer_TerrainTexture::CVIBuffer_TerrainTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_TerrainTexture::CVIBuffer_TerrainTexture(const CVIBuffer_TerrainTexture & other)
	: CVIBuffer(other)
	, m_dwVTXCntX(other.m_dwVTXCntX)
	, m_dwVTXCntZ(other.m_dwVTXCntZ)
	, m_fVTXItv(other.m_fVTXItv)
{
}

HRESULT CVIBuffer_TerrainTexture::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_TEXTURE);
	m_dwFVF = FVF_VTX_TEXTURE;
	m_dwVtxCount = m_dwVTXCntX * m_dwVTXCntZ;
	m_dwTriCount = (m_dwVTXCntX - 1) * (m_dwVTXCntZ - 1) * 2;

	m_dwIndexSize = sizeof(INDEX32);
	m_IndexFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	DWORD* pPixel = LoadHeightMap_FromFile(L"../Resources/Terrain/Height.bmp");
	if (nullptr == pPixel)
		return E_FAIL;

	// 버텍스 셋팅.
	VTX_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	DWORD dwIndex = 0;

	for (DWORD i = 0; i < m_dwVTXCntZ; ++i)
	{
		for (DWORD j = 0; j < m_dwVTXCntX; ++j)
		{
			dwIndex = i * m_dwVTXCntX + j;

			float y = (pPixel[dwIndex] & 0x000000ff) / 100.f;

			pVertex[dwIndex].vPosition = D3DXVECTOR3(j * m_fVTXItv, y, i * m_fVTXItv);
			pVertex[dwIndex].vUV = D3DXVECTOR2(j / (m_dwVTXCntX - 1.f), 1.f - i / (m_dwVTXCntZ - 1.f));
		}
	}

	Safe_Delete_Array(pPixel);
	m_pVB->Unlock();

	// 인덱스 셋팅
	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	DWORD dwTriIndex = 0;

	for (DWORD i = 0; i < m_dwVTXCntZ - 1; ++i)
	{
		for (DWORD j = 0; j < m_dwVTXCntX - 1; ++j)
		{
			dwIndex = i * m_dwVTXCntX + j;

			pIndex[dwTriIndex]._1 = dwIndex + m_dwVTXCntX;
			pIndex[dwTriIndex]._2 = dwIndex + m_dwVTXCntX + 1;
			pIndex[dwTriIndex++]._3 = dwIndex + 1;

			pIndex[dwTriIndex]._1 = dwIndex + m_dwVTXCntX;
			pIndex[dwTriIndex]._2 = dwIndex + 1;
			pIndex[dwTriIndex++]._3 = dwIndex;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::Render_VIBuffer()
{

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0, m_dwTriCount);

	return S_OK;
}

DWORD* CVIBuffer_TerrainTexture::LoadHeightMap_FromFile(const wstring & FilePath)
{
	HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed To LoadHeightMap_FromFile");
		return nullptr;
	}

	DWORD dwBytes = 0;

	BITMAPFILEHEADER tBitmapFileHeader;
	ZeroMemory(&tBitmapFileHeader, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER tBitmapInfoHeader;
	ZeroMemory(&tBitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	ReadFile(hFile, &tBitmapFileHeader, sizeof(BITMAPFILEHEADER), &dwBytes, nullptr);
	ReadFile(hFile, &tBitmapInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytes, nullptr);

	DWORD* pPixel = new DWORD[tBitmapInfoHeader.biWidth * tBitmapInfoHeader.biHeight];
	ReadFile(hFile, pPixel, sizeof(DWORD) * tBitmapInfoHeader.biWidth * tBitmapInfoHeader.biHeight, &dwBytes, nullptr);

	return pPixel;
}

CVIBuffer_TerrainTexture* CVIBuffer_TerrainTexture::Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwVTXCntX, DWORD dwVTXCntZ, float fVTXItv/* = 1.f*/)
{
	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(pDevice);
	pInstance->m_dwVTXCntX = dwVTXCntX;
	pInstance->m_dwVTXCntZ = dwVTXCntZ;
	pInstance->m_fVTXItv = fVTXItv;

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_TerrainTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_TerrainTexture::Clone_Component(void* pArg)
{
	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_TerrainTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TerrainTexture::Free()
{
	CVIBuffer::Free();
}
