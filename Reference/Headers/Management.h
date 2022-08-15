#pragma once

#ifndef __MANAGEMENT_H__

#include "Base.h"
#pragma region ENGINE_SYSTEM
#include "Graphic_Device.h"
#include "Time_Manager.h"
#include "Frame_Manager.h"
#include "Key_Manager.h"
#pragma endregion

#pragma region ENGINE_UTILITY
#include "Renderer.h"
#include "Scene_Manager.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Observer_Manager.h"
#include "Sound_Manager.h"
#pragma endregion

BEGIN(Engine)

class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement() = default;

public:
	// Getter
	int Get_Current_SceneID() const;
	float GeT_SlowTime() const;

	// Setter
	void Set_SlowTime(const float& _fSlowTime) { m_fSlowTime = _fSlowTime; }

public:
	//For.General
	HRESULT Ready_Engine(int iSceneCount, HWND hWnd, DWORD dwWinCX, DWORD dwWinCY, CGraphic_Device::WINMODE eWinMode);
	int Update_Engine();
	HRESULT Render_Engine();
	HRESULT Clear_ForScene(int iSceneID);

	//For.Graphic_Device
	LPDIRECT3DDEVICE9 Get_Device();
	HRESULT Draw_Font(TCHAR* _pString, RECT* _pRect, DWORD _dwColor);

	//For.Renderer
	HRESULT Add_RenderList(CRenderer::RENDER_GROUP eGroupID, CGameObject* pObject);
	HRESULT Set_OrthoMatrix(float _ScreenX, float _ScreenY);

	//For.Scene_Manager
	HRESULT SetUp_CurrentScene(int iCurrent_SceneID, CScene* pCurrentScene);
	CScene* Get_CurrentScene_Pointer();

	//For.Time_Manager
	float Get_DeltaTime();

	//For.Object_Manager
	CGameObject* Get_GameObject_Pointer(int iSceneID, const wstring& LayerTag, int iIndex = 0);
	CComponent* Get_Component_Pointer(int iSceneID, const wstring& LayerTag, const wstring& ComponentTag, int iObjectIndex = 0);
	HRESULT Add_GameObject_Prototype(int iSceneID, const wstring& GameObjectTag, CGameObject* pProtoType);
	HRESULT Add_GameObject_ToLayer(int iSceneID_FromPrototype, const wstring& GameObjectTag, int iSceneID_ToLayer, const wstring& LayerTag, void* pArg = nullptr);
	int Get_Layer_Size(int iSceneID, const wstring & LayerTag) const;

	//For.Component_Manager	
	HRESULT Add_Component_Prototype(int iSceneID, const wstring& ComponentTag, CComponent* pProtoType);
	CComponent* Clone_Component(int iSceneID, const wstring& ComponentTag, void* pArg);

	//For.Key_Manager
	bool KeyDown(int _iKey);
	bool KeyUp(int _iKey);
	bool KeyPressing(int _iKey);
	HRESULT KeyUpdate();

	//For.Observer_Manager
	HRESULT Subscribe(CObserver* _pObserver);
	HRESULT UnSubscribe(CObserver* _pObserver);
	HRESULT Notify(int _iMessage, void* _pData);

	//For.Sound_Manager
	HRESULT Play_Sound(TCHAR* pSoundKey, CSound_Manager::CHANNELID eID);
	HRESULT Play_BGM(TCHAR* pSoundKey);
	HRESULT StopSound(CSound_Manager::CHANNELID eID);
	HRESULT StopAll();

public:
	static void Release_Engine();
	virtual void Free();

private:
	CGraphic_Device*	m_pGraphic_Device = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CScene_Manager*		m_pScene_Manager = nullptr;
	CTime_Manager*		m_pTime_Manager = nullptr;
	CComponent_Manager*	m_pComponent_Manager = nullptr;
	CObject_Manager*	m_pObject_Manager = nullptr;
	CKey_Manager*		m_pKey_Manager = nullptr;
	CObserver_Manager*	m_pObserver_Manager = nullptr;
	CSound_Manager*		m_pSound_Manager = nullptr;
	int					m_iCurrent_SceneID = 0;
	float				m_fSlowTime = 1.f;
};

END

#define __MANAGEMENT_H__
#endif


