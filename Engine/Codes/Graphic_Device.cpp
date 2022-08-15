#include "Graphic_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_pDevice(nullptr)
	, m_pSDK(nullptr)
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, DWORD dwWinCX, DWORD dwWinCY, WINMODE eWinMode)
{
	D3DCAPS9 DeviceCap; 
	ZeroMemory(&DeviceCap, sizeof(D3DCAPS9)); 

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCap)))
	{
		ERR_MSG(L"Get Device Caps Failed - Graphic_Device.cpp "); 
		return E_FAIL; 
	}
	 
	DWORD vp = 0; 
	
	if (DeviceCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS)); 
	d3dpp.BackBufferWidth = dwWinCX; 
	d3dpp.BackBufferHeight = dwWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; 
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eWinMode; 
	
	d3dpp.EnableAutoDepthStencil = TRUE; 
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Device Create Failed"); 
		return E_FAIL; 
	}	

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 18;
	tFontInfo.Width = 13;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"±¼¸²");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Com Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphic_Device::Draw_Font(TCHAR * _pString, RECT * _pRect, DWORD _dwColor)
{
	m_pFont->DrawTextW(nullptr, _pString, lstrlen(_pString), _pRect, DT_CENTER, _dwColor);
	return S_OK;
}

void CGraphic_Device::Free()
{
	if (Safe_Release(m_pFont))
		ERR_MSG(L"Failed To Releasing m_pFont");

	if (Safe_Release(m_pDevice))
		ERR_MSG(L"Failed To Releasing m_pDevice");

	if (Safe_Release(m_pSDK))
		ERR_MSG(L"Failed To Releasing m_pSDK");
}
