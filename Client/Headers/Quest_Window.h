#pragma once

#ifndef __QUEST_WINDOW_H__
#define __QUEST_WINDOW_H__

#include "UserInterface.h"

BEGIN(Client)

class CQuest_Window final : public CUserInterface
{
public:
	enum QUEST_STATE
	{
		QUEST_ING, // 퀘스트 진행 중
		QUEST_CLEAR, // 퀘스트 클리어
		QUEST_FAIL // 퀘스트 실패
	};

private:
	explicit CQuest_Window(LPDIRECT3DDEVICE9 _pDevice);
	explicit CQuest_Window(const CQuest_Window& _other);
	virtual ~CQuest_Window() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	HRESULT SetUp_CurrentQuest();
	HRESULT SetUp_QuestCount(CManagement* _pManagement);
	HRESULT Render_QuestText();

public:
	static CQuest_Window* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CObserver*		m_pPlayer_Observer = nullptr;

	bool			m_bIsInit = false;

	TCHAR			m_szQuest_Text[MAX_PATH] = L""; // 퀘스트 내용
	QUEST_STATE		m_eQuestState = QUEST_ING; // 퀘스트 진행 상황

	int				m_iCurrent_QuestCount = 0; // 현재 퀘스트 진행 상황
	int				m_iClear_QuestCount = 0; // 퀘스트 완료 조건

	DWORD			m_dwColor = 0;
	float			m_fSecond = 0.f;
};

END

#endif // !__QUEST_WINDOW_H__


