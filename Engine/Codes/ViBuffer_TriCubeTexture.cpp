#include "..\Headers\VIBuffer_TriCubeTexture.h"

USING(Engine)

CVIBuffer_TriCubeTexture::CVIBuffer_TriCubeTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_TriCubeTexture::CVIBuffer_TriCubeTexture(const CVIBuffer_TriCubeTexture & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_TriCubeTexture::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_CUBE_TEXTURE);
	m_dwFVF = FVF_VTX_CUBE_TEXTURE;
	m_dwVtxCount = 8;
	m_dwTriCount = 12;

	m_dwIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_CUBE_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[0].vUV = pVertex[0].vPosition;

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].vUV = pVertex[1].vPosition;

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].vUV = pVertex[2].vPosition;

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vUV = pVertex[3].vPosition;

	pVertex[4].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[4].vUV = pVertex[4].vPosition;

	pVertex[5].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].vUV = pVertex[5].vPosition;

	pVertex[6].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].vUV = pVertex[6].vPosition;

	pVertex[7].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 면 삼각형 2개
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// 왼쪽 면 삼각형 2개
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// 위쪽 면 삼각형 2개
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// 아래쪽 면 삼각형 2개
	pIndex[6]._1 = 6;
	pIndex[6]._2 = 7;
	pIndex[6]._3 = 3;

	pIndex[7]._1 = 6;
	pIndex[7]._2 = 3;
	pIndex[7]._3 = 2;

	// 앞쪽 면 삼각형 2개
	pIndex[8]._1 = 0;
	pIndex[8]._2 = 1;
	pIndex[8]._3 = 2;

	pIndex[9]._1 = 0;
	pIndex[9]._2 = 2;
	pIndex[9]._3 = 3;

	// 뒷쪽 면 삼각형 2개
	pIndex[10]._1 = 5;
	pIndex[10]._2 = 4;
	pIndex[10]._3 = 7;

	pIndex[11]._1 = 5;
	pIndex[11]._2 = 7;
	pIndex[11]._3 = 6;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TriCubeTexture::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TriCubeTexture::Render_VIBuffer()
{
	
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);	
	m_pDevice->SetIndices(m_pIB);	
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0, m_dwTriCount);

	return S_OK;
}

CVIBuffer_TriCubeTexture* CVIBuffer_TriCubeTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_TriCubeTexture* pInstance = new CVIBuffer_TriCubeTexture(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_TriCubeTexture::Clone_Component(void* pArg)
{
	CVIBuffer_TriCubeTexture* pInstance = new CVIBuffer_TriCubeTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TriCubeTexture::Free()
{
	CVIBuffer::Free();
}
