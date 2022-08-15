#include "..\Headers\Renderer.h"

USING(Engine)

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pDevice(pGraphicDev)
{
	Safe_AddRef(m_pDevice);
}

HRESULT CRenderer::Add_RenderList(RENDER_GROUP eGroupID, CGameObject* pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	m_RenderList[eGroupID].push_back(pObject);
	Safe_AddRef(pObject);

	return S_OK;
}

HRESULT CRenderer::Set_OrthoMatrix(float _ScreenX, float _ScreenY)
{
	D3DXMatrixOrthoLH(&m_matOrthoProj, _ScreenX, _ScreenY, 0.f, 1.f);
	return S_OK;
}

HRESULT CRenderer::Render()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matOrthoProj);
	if (FAILED(Render_UI()))
		return E_FAIL;

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pObject : m_RenderList[RENDER_PRIORITY])
	{
		if (FAILED(pObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pObject);
	}

	m_RenderList[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	for (auto& pObject : m_RenderList[RENDER_NONALPHA])
	{
		if (FAILED(pObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pObject);
	}

	m_RenderList[RENDER_NONALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObject : m_RenderList[RENDER_ALPHA])
	{
		if (FAILED(pObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pObject);
	}

	m_RenderList[RENDER_ALPHA].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pObject : m_RenderList[RENDER_UI])
	{
		if (FAILED(pObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pObject);
	}

	m_RenderList[RENDER_UI].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	return new CRenderer(pDevice);
}

void CRenderer::Free()
{
	for (int i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObject : m_RenderList[i])
			Safe_Release(pObject);

		m_RenderList[i].clear();
	}

	Safe_Release(m_pDevice);
}
