#include "stdafx.h"
#include "..\Headers\Quest_Window.h"

#include "Player_Observer.h"
#include "Player.h"
#include "Static_Camera.h"
#include "Scene_Stage.h"

USING(Client)

CQuest_Window::CQuest_Window(LPDIRECT3DDEVICE9 _pDevice)
	: CUserInterface(_pDevice)
{
}

CQuest_Window::CQuest_Window(const CQuest_Window & _other)
	: CUserInterface(_other)
{
}

HRESULT CQuest_Window::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CQuest_Window::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vScale = { float(WINCX >> 2), float(WINCY >> 2), 0.f };
	m_vPos = { float(WINCX) / 3.f + 60.f, float(WINCY) / 3.f + 60.f, 0.f };

	m_pPlayer_Observer = new CPlayer_Observer;
	CManagement::Get_Instance()->Subscribe(m_pPlayer_Observer);

	return S_OK;
}

int CQuest_Window::Update_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return 0;

	if (false == m_bIsInit || CPlayer::PLAYER_DEAD == pPlayer->Get_State())
	{
		if (FAILED(SetUp_CurrentQuest()))
			return 0;

		m_bIsInit = true;
	}

	CScene_Stage* pScene = (CScene_Stage*)pManagement->Get_CurrentScene_Pointer();
	if (nullptr == pScene)
		return 0;

	SCENE_ID eCurrentSceneID = (SCENE_ID)pManagement->Get_Current_SceneID();
	bool bUpdate_Time = false;

	switch (eCurrentSceneID)
	{
	case SCENE_STAGE2:
	case SCENE_STAGE3:
		bUpdate_Time = true;
		break;
	case SCENE_STAGE4:
		bUpdate_Time = !pScene->Get_StageKey();
		break;
	case SCENE_STAGE6: // 플레이어 이동
		bUpdate_Time = bool(CPlayer::PLAYER_IDLE == pPlayer->Get_State());

		if (false == bUpdate_Time)
		{
			m_fSecond = 0.f;
			m_iCurrent_QuestCount = 0;
		}
		break;
	case SCENE_STAGE7: // 파레트 시간
		bUpdate_Time = pPlayer->Get_Palette()->Get_Show();

		if (false == bUpdate_Time)
		{
			m_fSecond = 0.f;
			m_iCurrent_QuestCount = 0;
		}
		break;
	default:
		break;
	}

	if (true == bUpdate_Time)
	{
		if (FAILED(SetUp_QuestCount(pManagement)))
			return 0;
	}

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	m_matWorld = matScale * matTrans;

	return 0;
}

int CQuest_Window::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SCENE_ID eCurrentSceneID = (SCENE_ID)pManagement->Get_Current_SceneID();

	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pPlayer_Observer)->Get_Color())
	{
		m_dwColor = dynamic_cast<CPlayer_Observer*>(m_pPlayer_Observer)->Get_Color();

		if ((SCENE_STAGE1 == eCurrentSceneID || SCENE_STAGE5 == eCurrentSceneID)
			&& m_dwColor != 0xff505050)
			++m_iCurrent_QuestCount;
	}

	if (m_iCurrent_QuestCount > m_iClear_QuestCount) // 색깔을 많이 바꿈
		m_eQuestState = QUEST_FAIL;
	else
		m_eQuestState = QUEST_CLEAR;

	if (QUEST_FAIL == m_eQuestState)
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
		if (nullptr == pPlayer)
			return 0;

		pPlayer->Set_Player_State(CPlayer::PLAYER_DEAD);
	}

	return 	CUserInterface::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CQuest_Window::Render_GameObject()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	if (SCENE_FINAL == pManagement->Get_Current_SceneID())
		return S_OK;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파블렌딩 활성화 on.

	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
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
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파블렌딩 활성화 on.

	if (FAILED(Render_QuestText()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuest_Window::Add_Component()
{
	if (FAILED(CUserInterface::Add_Component()))
		return E_FAIL;

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Quest_Window",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CQuest_Window::SetUp_CurrentQuest()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SCENE_ID eCurrentSceneID = (SCENE_ID)pManagement->Get_Current_SceneID();

	switch (eCurrentSceneID)
	{
	case SCENE_STAGE1:
		lstrcpy(m_szQuest_Text, L"주어진 횟수 제한 이내로 색상을 바꿔 스테이지를 클리어 하시오.");
		m_iCurrent_QuestCount = 0;
		m_iClear_QuestCount = 8;
		break;
	case SCENE_STAGE2:
		lstrcpy(m_szQuest_Text, L"제한 시간 내에 클리어 하시오.");
		m_fSecond = 0.f;
		m_iCurrent_QuestCount = 0; // 시간 흐름
		m_iClear_QuestCount = 120; // 제한 시간
		break;	
	case SCENE_STAGE3:
		lstrcpy(m_szQuest_Text, L"제한 시간 내에 클리어 하시오.");
		m_fSecond = 0.f;
		m_iCurrent_QuestCount = 0; // 시간 흐름
		m_iClear_QuestCount = 30; // 제한 시간
		break;
	case SCENE_STAGE4:
		lstrcpy(m_szQuest_Text, L"제한 시간 내에 열쇠를 획득하시오.");
		m_fSecond = 0.f;
		m_iCurrent_QuestCount = 0; // 시간 흐름
		m_iClear_QuestCount = 60; // 제한 시간
		break;
	case SCENE_STAGE5:
		lstrcpy(m_szQuest_Text, L"주어진 횟수 제한 이내로 색상을 바꿔 스테이지를 클리어 하시오.");
		m_iCurrent_QuestCount = 0;
		m_iClear_QuestCount = 6;
		break;
	case SCENE_STAGE6:
		lstrcpy(m_szQuest_Text, L"이동을 5초 이상 하지 않으면 죽습니다.");
		m_iCurrent_QuestCount = 0;
		m_iClear_QuestCount = 5;
		break;
	case SCENE_STAGE7:
		lstrcpy(m_szQuest_Text, L"파레트를 8초 이상 켜면 죽습니다.");
		m_iCurrent_QuestCount = 0;
		m_iClear_QuestCount = 8;
		break;
	case SCENE_STAGE8:
		lstrcpy(m_szQuest_Text, L"시연회를 끝내시오.");
		m_iCurrent_QuestCount = 0;
		m_iClear_QuestCount = 0;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CQuest_Window::SetUp_QuestCount(CManagement * _pManagement)
{
	CStatic_Camera* pCamera = (CStatic_Camera*)_pManagement->Get_GameObject_Pointer(_pManagement->Get_Current_SceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (CAM_EFFECT_NORMAL == pCamera->Get_CameraEffect())
	{
		float fRealTimeDelta = _pManagement->Get_DeltaTime();
		m_fSecond += fRealTimeDelta;

		m_iCurrent_QuestCount = (int)m_fSecond;
	}

	return S_OK;
}

HRESULT CQuest_Window::Render_QuestText()
{
	// 퀘스트 출력
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// 퀘스트 내용 출력
	wstring wstrBuff = m_szQuest_Text;
	TCHAR szBuf[64] = L"";
	int iStart = 0;
	int iLength = lstrlen(m_szQuest_Text);
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	RECT rc = { LONG(m_vPos.x - m_vScale.x / 2.f + float(WINCX >> 1)), LONG(m_vPos.y - m_vScale.y / 2.f - float(WINCY / 6))
		, LONG(m_vPos.x + m_vScale.x / 2.f + float(WINCX >> 1)), LONG(m_vPos.y + m_vScale.y / 2.f - float(WINCY / 6)) };


	if (iLength > 18)
	{
		while (iStart < iLength - 18)
		{
			wsprintf(szBuf, wstrBuff.substr(iStart, 18).c_str());

			if (FAILED(pManagement->Draw_Font(szBuf, &rc, D3DCOLOR_ARGB(255, 255, 255, 255))))
				return E_FAIL;

			iStart += 18;
			rc.top += 22; rc.bottom += 22;
		}
	}

	wsprintf(szBuf, wstrBuff.substr(iStart, iLength).c_str());

	if (FAILED(pManagement->Draw_Font(szBuf, &rc, D3DCOLOR_ARGB(255, 255, 255, 255))))
		return E_FAIL;

	// 퀘스트 진행상황 출력
	if (0 < m_iClear_QuestCount)
	{
		DWORD dwTextColor = 0;

		switch (m_eQuestState)
		{
		case QUEST_ING:
			dwTextColor = D3DCOLOR_ARGB(255, 255, 255, 255);
			break;
		case QUEST_CLEAR:
			dwTextColor = D3DCOLOR_ARGB(255, 0, 255, 0);
			break;
		case QUEST_FAIL:
			dwTextColor = D3DCOLOR_ARGB(255, 255, 0, 0);
			break;
		}

		wsprintf(szBuf, L"%d / %d", m_iCurrent_QuestCount, m_iClear_QuestCount);

		rc.left += 120;
		rc.top += 80;
		rc.right += 120;
		rc.bottom += 80;

		if (FAILED(pManagement->Draw_Font(szBuf, &rc, dwTextColor)))
			return E_FAIL;
	}

	return S_OK;
}

CQuest_Window * CQuest_Window::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CQuest_Window* pInstance = new CQuest_Window(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CQuest_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuest_Window::Clone_GameObject(void * pArg)
{
	CQuest_Window* pInstance = new CQuest_Window(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CQuest_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuest_Window::Free()
{
	Safe_Release(m_pPlayer_Observer);

	CUserInterface::Free();
}