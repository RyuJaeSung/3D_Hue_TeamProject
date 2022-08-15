#include "..\Headers\VIBuffer_RectTexture.h"

USING(Engine)

CVIBuffer_RectTexture::CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_RectTexture::CVIBuffer_RectTexture(const CVIBuffer_RectTexture & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_RectTexture::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_TEXTURE);
	m_dwFVF = FVF_VTX_TEXTURE;
	m_dwVtxCount = 4;
	m_dwTriCount = 2;

	m_dwIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 버텍스 셋팅.
	VTX_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[0].vUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[1].vUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[2].vUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertex[3].vUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	// 인덱스 셋팅
	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_RectTexture::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_RectTexture::Render_VIBuffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0, m_dwTriCount);

	return S_OK;
}

CVIBuffer_RectTexture* CVIBuffer_RectTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_RectTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RectTexture::Clone_Component(void* pArg)
{
	CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_RectTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectTexture::Free()
{
	CVIBuffer::Free();
}
