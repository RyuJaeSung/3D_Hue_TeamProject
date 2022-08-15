#include "..\Headers\VIBuffer_TerrainColor.h"

USING(Engine)

CVIBuffer_TerrainColor::CVIBuffer_TerrainColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_TerrainColor::CVIBuffer_TerrainColor(const CVIBuffer_TerrainColor & other)
	: CVIBuffer(other)
	, m_dwVTXCntX(other.m_dwVTXCntX)
	, m_dwVTXCntZ(other.m_dwVTXCntZ)
	, m_fVTXItv(other.m_fVTXItv)
{
}

HRESULT CVIBuffer_TerrainColor::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_COLOR);
	m_dwFVF = FVF_VTX_COLOR;
	m_dwVtxCount = m_dwVTXCntX * m_dwVTXCntZ;
	m_dwTriCount = (m_dwVTXCntX - 1) * (m_dwVTXCntZ - 1) * 2;

	m_dwIndexSize = sizeof(INDEX32);
	m_IndexFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 버텍스 셋팅.
	VTX_COLOR* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	DWORD dwIndex = 0;

	for (DWORD i = 0; i < m_dwVTXCntZ; ++i)
	{
		for (DWORD j = 0; j < m_dwVTXCntX; ++j)
		{
			dwIndex = i * m_dwVTXCntX + j;

			pVertex[dwIndex].vPosition = D3DXVECTOR3(j * m_fVTXItv, 0.f, i * m_fVTXItv);
			pVertex[dwIndex].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

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

HRESULT CVIBuffer_TerrainColor::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TerrainColor::Render_VIBuffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0, m_dwTriCount);

	return S_OK;
}

CVIBuffer_TerrainColor* CVIBuffer_TerrainColor::Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwVTXCntX, DWORD dwVTXCntZ, float fVTXItv/* = 1.f*/)
{
	CVIBuffer_TerrainColor* pInstance = new CVIBuffer_TerrainColor(pDevice);
	pInstance->m_dwVTXCntX = dwVTXCntX;
	pInstance->m_dwVTXCntZ = dwVTXCntZ;
	pInstance->m_fVTXItv = fVTXItv;

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_TerrainColor");
		Safe_Release(pInstance);
	}	

	return pInstance;
}

CComponent* CVIBuffer_TerrainColor::Clone_Component(void* pArg)
{
	CVIBuffer_TerrainColor* pInstance = new CVIBuffer_TerrainColor(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_TerrainColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TerrainColor::Free()
{
	CVIBuffer::Free();
}
