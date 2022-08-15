#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE { WINMODE_FULL, WINMODE_WIN };
private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	LPDIRECT3DDEVICE9	Get_Device()	{ return m_pDevice;	}
	
public:
	HRESULT				Ready_Graphic_Device(HWND hWnd, DWORD dwWinCX, DWORD dwWinCY, WINMODE eWinMode);
	HRESULT				Draw_Font(TCHAR* _pString, RECT* _pRect, DWORD _dwColor);

public:
	virtual void Free() override;

private:
	LPDIRECT3D9			m_pSDK; 
	LPDIRECT3DDEVICE9	m_pDevice;

	LPD3DXFONT			m_pFont;
};

END