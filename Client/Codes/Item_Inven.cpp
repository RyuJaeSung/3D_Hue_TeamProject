#include "stdafx.h"
#include "..\Headers\Item_Inven.h"

USING(Client)

CItem_Inven::CItem_Inven(LPDIRECT3DDEVICE9 _pDevice)
	: CUserInterface(_pDevice)
{
}

CItem_Inven::CItem_Inven(const CItem_Inven & _other)
	: CUserInterface(_other)
{
	for (int i = 0; i < ITEM_END; ++i)
		m_bGetItem[i] = false;
}

HRESULT CItem_Inven::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItem_Inven::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vScale = { 130.f / 3.f, 130.f / 3.f, 0.f };

	// 제일 왼쪽 아이템 위치
	m_vPos = { float(WINCX >> 1) - 420.f, float(WINCY >> 1) - 50.f, 0.f };

	// 아이템 획득 했는지, 안했는지 확인
	if (nullptr != pArg)
		memcpy(m_bGetItem, pArg, sizeof(bool) * ITEM_END);

	return S_OK;
}

int CItem_Inven::Update_GameObject(float fTimeDelta)
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	m_matWorld = matScale * matTrans;

	return 0;
}

int CItem_Inven::LateUpdate_GameObject(float fTimeDelta)
{
	return 	CUserInterface::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CItem_Inven::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파블렌딩 활성화 on.
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXMATRIX matBg = m_matWorld;
	matBg._11 += 0.03f;
	matBg._22 += 0.03f;

	D3DXMATRIX matItem = m_matWorld;

	DWORD dwColor = 0;

	for (int i = 0; i < ITEM_END; ++i)
	{
		if (true == m_bGetItem[i])
		{
			// 아이템 배경을 그린다
			m_pDevice->SetTransform(D3DTS_WORLD, &matBg);

			if (FAILED(m_pItemTextureCom[TEXTURE_BG]->Set_Texture(0)))
				return E_FAIL;
			if (FAILED(m_pItemVIBufferCom[TEXTURE_BG]->Render_VIBuffer()))
				return E_FAIL;
		}

		// 아이템 몸통 랜더
		m_pDevice->SetTransform(D3DTS_WORLD, &matItem);
		dwColor = SetUp_ItemColor((ITEM_COLOR)i);

		m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		if (FAILED(m_pTextureCom->Set_Texture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;

		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		if (true == m_bGetItem[i])
		{
			// 고글을 그린다
			if (FAILED(m_pItemTextureCom[TEXTURE_GLASS]->Set_Texture(0)))
				return E_FAIL;
			if (FAILED(m_pItemVIBufferCom[TEXTURE_GLASS]->Render_VIBuffer()))
				return E_FAIL;
			//
		}

		matBg._41 += m_vScale.x + 5.f;
		matItem._41 += m_vScale.x + 5.f;
	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파블렌딩 활성화 on.

	return S_OK;
}

HRESULT CItem_Inven::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer_Glass", (CComponent**)&m_pItemVIBufferCom[TEXTURE_GLASS])))
		return E_FAIL;
#pragma endregion

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer_Bg", (CComponent**)&m_pItemVIBufferCom[TEXTURE_BG])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_UI",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_Glass",
		L"Com_Texture_Glass", (CComponent**)&m_pItemTextureCom[TEXTURE_GLASS])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_Bg",
		L"Com_Texture_Bg", (CComponent**)&m_pItemTextureCom[TEXTURE_BG])))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

DWORD CItem_Inven::SetUp_ItemColor(const ITEM_COLOR& _eItemColor)
{
	int iAlpha = 0;

	// 아이템을 얻은 상태
	if (true == m_bGetItem[_eItemColor])
		iAlpha = 255;
	else
		iAlpha = 100;

	switch (_eItemColor)
	{
	case ITEM_RED:
		return D3DCOLOR_RGBA(246, 19, 28, iAlpha);
	case ITEM_ORANGE:
		return D3DCOLOR_RGBA(255, 140, 27, iAlpha);
	case ITEM_YELLOW:
		return D3DCOLOR_RGBA(249, 196, 22, iAlpha);
	case ITEM_GREEN:	
		return D3DCOLOR_RGBA(145, 238, 0, iAlpha);
	case ITEM_BLUE:
		return D3DCOLOR_RGBA(18, 179, 221, iAlpha);
	case ITEM_NAVY:	
		return D3DCOLOR_RGBA(43, 55, 251, iAlpha);
	case ITEM_PURPLE:
		return D3DCOLOR_RGBA(168, 22, 251, iAlpha);
	case ITEM_PINK:
		return D3DCOLOR_RGBA(239, 68, 170, iAlpha);
	}

	return 0;
}

CItem_Inven * CItem_Inven::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CItem_Inven* pInstance = new CItem_Inven(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CItem_Inven");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Inven::Clone_GameObject(void * pArg)
{
	CItem_Inven* pInstance = new CItem_Inven(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CItem_Inven");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Inven::Free()
{
	for (int i = 0; i < TEXTURE_END; ++i)
	{
		Safe_Release(m_pItemVIBufferCom[i]);
		Safe_Release(m_pItemTextureCom[i]);
	}

	CUserInterface::Free();
}
