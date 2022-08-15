#pragma once

#include "Base.h"

BEGIN(Client)

class CMainApp final : public Engine::CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	int Update_MainApp();
	HRESULT Render_MainApp();

private:
	HRESULT Ready_MainApp();
	HRESULT Ready_Static_Resources();
	HRESULT Ready_Default_Setting();

public:
	static CMainApp* Create();
	virtual void Free() override;

private:
	CManagement*		m_pManagement = nullptr;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
};

END

