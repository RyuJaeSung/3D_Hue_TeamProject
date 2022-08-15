#include "..\Headers\Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CTexture::CTexture(const CTexture & other)
	: CComponent(other)
	, m_Textures(other.m_Textures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CTexture::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Ready_Component_Prototype(TEXTURE_TYPE eType, const wstring & FilePath, int iCount)
{
	IDirect3DBaseTexture9* pTexture = nullptr;
	TCHAR szFullPath[MIN_STR] = L"";

	m_Textures.reserve(iCount);

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, FilePath.c_str(), i);

		switch (eType)
		{
		case Engine::CTexture::TEXTURE_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case Engine::CTexture::TEXTURE_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;		
		}

		m_Textures.push_back(pTexture);
	}
	
	return S_OK;
}

HRESULT CTexture::Set_Texture(int iIndex)
{
	if (0 > iIndex ||
		m_Textures.size() <= (size_t)iIndex)
		return E_FAIL;

	if (nullptr == m_Textures[iIndex])
		return E_FAIL;

	if (FAILED(m_pDevice->SetTexture(0, m_Textures[iIndex])))
		return E_FAIL;

	return S_OK;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pDevice, TEXTURE_TYPE eType, const wstring& FilePath, int iCount)
{
	CTexture* pInstance = new CTexture(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype(eType, FilePath, iCount)))
	{
		ERR_MSG(FilePath.c_str());
		ERR_MSG(L"Failed To Load Texture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone_Component(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{		
		ERR_MSG(L"Failed To Clone CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
	m_Textures.shrink_to_fit();

	CComponent::Free();
}
