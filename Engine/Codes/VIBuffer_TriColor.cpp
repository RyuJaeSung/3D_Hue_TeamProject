#include "..\Headers\VIBuffer_TriColor.h"

USING(Engine)

CVIBuffer_TriColor::CVIBuffer_TriColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_TriColor::CVIBuffer_TriColor(const CVIBuffer_TriColor & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_TriColor::Ready_Component_Prototype()
{
	m_dwVtxSize = sizeof(VTX_COLOR);
	m_dwFVF = FVF_VTX_COLOR;
	m_dwVtxCount = 3;
	m_dwTriCount = 1;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	// 버텍스 셋팅.
	VTX_COLOR* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[1].vPosition = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[2].vPosition = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TriColor::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TriColor::Render_VIBuffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCount);

	return S_OK;
}

CVIBuffer_TriColor* CVIBuffer_TriColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_TriColor* pInstance = new CVIBuffer_TriColor(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CVIBuffer_TriColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_TriColor::Clone_Component(void* pArg)
{
	CVIBuffer_TriColor* pInstance = new CVIBuffer_TriColor(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CVIBuffer_TriColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TriColor::Free()
{
	CVIBuffer::Free();
}
