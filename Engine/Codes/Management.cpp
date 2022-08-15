#include "..\Headers\Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
	, m_pTime_Manager(CTime_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pObserver_Manager(CObserver_Manager::Get_Instance())
	, m_pSound_Manager(CSound_Manager::Get_Instance())
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pScene_Manager);
	Safe_AddRef(m_pTime_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pObserver_Manager);
	Safe_AddRef(m_pSound_Manager);
}

int CManagement::Get_Current_SceneID() const
{
	return m_iCurrent_SceneID;
}

float CManagement::GeT_SlowTime() const
{
	return m_fSlowTime;
}

HRESULT CManagement::Ready_Engine(int iSceneCount, HWND hWnd, DWORD dwWinCX, DWORD dwWinCY, CGraphic_Device::WINMODE eWinMode)
{
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(hWnd, dwWinCX, dwWinCY, eWinMode)))
	{
		ERR_MSG(L"Failed To Ready_Graphic_Device");
		return E_FAIL;
	}

	if (FAILED(m_pTime_Manager->Ready_Time_Manager()))
	{
		ERR_MSG(L"Failed To Ready_Time_Manager");
		return E_FAIL;
	}

	m_pRenderer = CRenderer::Create(m_pGraphic_Device->Get_Device());
	if (nullptr == m_pRenderer)
	{
		ERR_MSG(L"Failed To Creaing CRenderer");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Reserve_Container(iSceneCount)))
	{
		ERR_MSG(L"Failed To Reserve_Container CComponent_Manager");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Reserve_Container(iSceneCount)))
	{
		ERR_MSG(L"Failed To Reserve_Container CObject_Manager");
		return E_FAIL;
	}

	if (FAILED(m_pSound_Manager->Ready_Sound_Manager()))
	{
		ERR_MSG(L"Failed  To Ready CSound_Manager");
		return E_FAIL;
	}

	return S_OK;
}

int CManagement::Update_Engine()
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pScene_Manager)
		return 0;

	float fTimeDelta = m_pTime_Manager->Update_Time() * m_fSlowTime;

	if (m_pObject_Manager->Update_Object_Manager(fTimeDelta))
		return 0;

	if (m_pScene_Manager->Update_Scene(fTimeDelta))
		return 0;

	if (m_pObject_Manager->LateUpdate_Object_Manager(fTimeDelta))
		return 0;

	if (m_pScene_Manager->LateUpdate_Scene(fTimeDelta))
		return 0;

	m_pKey_Manager->KeyUpdate();

	return 0;
}

HRESULT CManagement::Render_Engine()
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Render();
}

HRESULT CManagement::Clear_ForScene(int iSceneID)
{
	if (nullptr == m_pComponent_Manager ||
		nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_ForScene(iSceneID)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_ForScene(iSceneID)))
		return E_FAIL;

	return S_OK;
}

LPDIRECT3DDEVICE9 CManagement::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Device();
}

HRESULT CManagement::Draw_Font(TCHAR * _pString, RECT * _pRect, DWORD _dwColor)
{
	return m_pGraphic_Device->Draw_Font(_pString, _pRect, _dwColor);
}

HRESULT CManagement::Add_RenderList(CRenderer::RENDER_GROUP eGroupID, CGameObject * pObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderList(eGroupID, pObject);
}

HRESULT CManagement::Set_OrthoMatrix(float _ScreenX, float _ScreenY)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Set_OrthoMatrix(_ScreenX, _ScreenY);
}

HRESULT CManagement::SetUp_CurrentScene(int iCurrent_SceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pScene_Manager)
		return E_FAIL;

	m_iCurrent_SceneID = iCurrent_SceneID;

	return m_pScene_Manager->SetUp_CurrentScene(pCurrentScene);
}

CScene * CManagement::Get_CurrentScene_Pointer()
{
	if (nullptr == m_pScene_Manager)
		return nullptr;

	return m_pScene_Manager->Get_CurrentScene_Pointer();
}

float CManagement::Get_DeltaTime()
{
	if (nullptr == m_pTime_Manager)
		return 0.f;

	return m_pTime_Manager->Get_DeltaTime();
}

CGameObject * CManagement::Get_GameObject_Pointer(int iSceneID, const wstring & LayerTag, int iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject_Pointer(iSceneID, LayerTag, iIndex);
}

CComponent * CManagement::Get_Component_Pointer(int iSceneID, const wstring & LayerTag, const wstring & ComponentTag, int iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component_Pointer(iSceneID, LayerTag, ComponentTag, iObjectIndex);
}

HRESULT CManagement::Add_GameObject_Prototype(int iSceneID, const wstring & GameObjectTag, CGameObject * pProtoType)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_Prototype(iSceneID, GameObjectTag, pProtoType);
}

HRESULT CManagement::Add_GameObject_ToLayer(int iSceneID_FromPrototype, const wstring& GameObjectTag, int iSceneID_ToLayer, const wstring& LayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iSceneID_FromPrototype, GameObjectTag, iSceneID_ToLayer, LayerTag, pArg);
}

int CManagement::Get_Layer_Size(int iSceneID, const wstring & LayerTag) const
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (nullptr == m_pObject_Manager->Get_GameObject_List(iSceneID, LayerTag))
		return -1;

	return (int)m_pObject_Manager->Get_GameObject_List(iSceneID, LayerTag)->size();
	// TODO: 여기에 반환 구문을 삽입합니다.
}

HRESULT CManagement::Add_Component_Prototype(int iSceneID, const wstring & ComponentTag, CComponent * pProtoType)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Component_Prototype(iSceneID, ComponentTag, pProtoType);
}

CComponent * CManagement::Clone_Component(int iSceneID, const wstring & ComponentTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iSceneID, ComponentTag, pArg);
}

bool CManagement::KeyDown(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;
	return m_pKey_Manager->KeyDown(_iKey);
}

bool CManagement::KeyUp(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;
	return m_pKey_Manager->KeyUp(_iKey);
}

bool CManagement::KeyPressing(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;
	return m_pKey_Manager->KeyPressing(_iKey);
}

HRESULT CManagement::KeyUpdate()
{
	if (nullptr == m_pKey_Manager)
		return E_FAIL;
	m_pKey_Manager->KeyUpdate();
	return S_OK;
}

HRESULT CManagement::Subscribe(CObserver * _pObserver)
{
	if (nullptr == m_pObserver_Manager)
		return E_FAIL;
	m_pObserver_Manager->Subscribe(_pObserver);
	return S_OK;
}

HRESULT CManagement::UnSubscribe(CObserver * _pObserver)
{
	if (nullptr == m_pObserver_Manager)
		return E_FAIL;
	m_pObserver_Manager->Unsubscribe(_pObserver);
	return S_OK;
}

HRESULT CManagement::Notify(int _iMessage, void * _pData)
{
	if (nullptr == m_pObserver_Manager)
		return E_FAIL;
	m_pObserver_Manager->Notify(_iMessage, _pData);
	return S_OK;
}

HRESULT CManagement::Play_Sound(TCHAR * pSoundKey, CSound_Manager::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->PlaySound(pSoundKey, eID);
	return S_OK;
}

HRESULT CManagement::Play_BGM(TCHAR * pSoundKey)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->PlayBGM(pSoundKey);
	return S_OK;
}

HRESULT CManagement::StopSound(CSound_Manager::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->StopSound(eID);
	return S_OK;
}

HRESULT CManagement::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->StopAll();
	return S_OK;
}

void CManagement::Release_Engine()
{
	DWORD dwRefCnt = 0;

	if (dwRefCnt = CManagement::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CManagement");

	if (dwRefCnt = CObject_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CObject_Manager");

	if (dwRefCnt = CObserver_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CObserver_Manager");

	if (dwRefCnt = CComponent_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CComponent_Manager");

	if (dwRefCnt = CTime_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CTime_Manager");

	if (dwRefCnt = CScene_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CScene_Manager");

	if (dwRefCnt = CGraphic_Device::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CGraphic_Device");

	if (dwRefCnt = CSound_Manager::Get_Instance()->Destroy_Instance())
		ERR_MSG(L"Failed To Destroy_Instance CSound_Manager");
}

void CManagement::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pScene_Manager);
	Safe_Release(m_pTime_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pObserver_Manager);
	Safe_Release(m_pSound_Manager);
}