#include "..\Headers\VIBuffer_RectColor.h"

USING(Engine)

CVIBuffer_RectColor::CVIBuffer_RectColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_RectColor::CVIBuffer_RectColor(const CVIBuffer_RectColor & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_RectColor::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_COLOR);
	m_dwFVF = FVF_VTX_COLOR;
	m_dwVtxCount = 4;
	m_dwTriCount = 2;

	m_dwIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 버텍스 셋팅.
	VTX_COLOR* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);	

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

HRESULT CVIBuffer_RectColor::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_RectColor::Render_VIBuffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0, m_dwTriCount);

	return S_OK;
}

CVIBuffer_RectColor* CVIBuffer_RectColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_RectColor* pInstance = new CVIBuffer_RectColor(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_RectColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RectColor::Clone_Component(void* pArg)
{
	CVIBuffer_RectColor* pInstance = new CVIBuffer_RectColor(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_RectColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectColor::Free()
{
	CVIBuffer::Free();
}
