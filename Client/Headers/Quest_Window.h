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
		QUEST_ING, // ����Ʈ ���� ��
		QUEST_CLEAR, // ����Ʈ Ŭ����
		QUEST_FAIL // ����Ʈ ����
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

	TCHAR			m_szQuest_Text[MAX_PATH] = L""; // ����Ʈ ����
	QUEST_STATE		m_eQuestState = QUEST_ING; // ����Ʈ ���� ��Ȳ

	int				m_iCurrent_QuestCount = 0; // ���� ����Ʈ ���� ��Ȳ
	int				m_iClear_QuestCount = 0; // ����Ʈ �Ϸ� ����

	DWORD			m_dwColor = 0;
	float			m_fSecond = 0.f;
};

END

#endif // !__QUEST_WINDOW_H__


