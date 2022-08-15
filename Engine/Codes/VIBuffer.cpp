#include "..\Headers\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & other)
	: CComponent(other)
	, m_pVB(other.m_pVB)
	, m_pIB(other.m_pIB)
	, m_dwFVF(other.m_dwFVF)
	, m_dwVtxCount(other.m_dwVtxCount)
	, m_dwVtxSize(other.m_dwVtxSize)
	, m_dwTriCount(other.m_dwTriCount)
	, m_dwIndexSize(other.m_dwIndexSize)
	, m_IndexFmt(other.m_IndexFmt)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Get_Vertices(void** pOut)
{
	if (nullptr == m_pVB ||
		nullptr == pOut)
		return E_FAIL;

	void* pVertices = nullptr;

	m_pVB->Lock(0, 0, &pVertices, 0);

	memcpy(*pOut, pVertices, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer::Set_Vertices(const void* pIn)
{
	if (nullptr == m_pVB ||
		nullptr == pIn)
		return E_FAIL;

	void* pVertices = nullptr;

	m_pVB->Lock(0, 0, &pVertices, 0);

	memcpy(pVertices, pIn, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer::Ready_Component_Prototype()
{
	
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCount,
		0, 
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		nullptr)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_dwIndexSize * m_dwTriCount,
		0,
		m_IndexFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Ready_Component(void * pArg)
{
	return S_OK;
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
