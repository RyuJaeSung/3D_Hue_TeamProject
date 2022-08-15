#pragma once
#ifndef __NON_PLAYER_CHARACTER_H__

#include "GameObject.h"
class CNon_Player_Character final : public CGameObject
{
public:
	enum NPC_PARTS	{ NPC_PARTS_HEAD, NPC_PARTS_BODY, NPC_PARTS_END};
	enum NPC_STATE	{ NPC_STATE_NORMAL, NPC_STATE_ANGRY, NPC_STATE_GOOD, NPC_STATE_END };
	enum NPC_TALK	{ NPC_TALK_1, NPC_TALK_2, NPC_TALK_WAITING, NPC_TALK_END};
public:
	explicit CNon_Player_Character(LPDIRECT3DDEVICE9 _pDevice);
	explicit CNon_Player_Character(const CNon_Player_Character& _other);
	virtual ~CNon_Player_Character() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	//Getter
	bool Is_Able_to_Talk() { return m_bAbletoTalk; }

private:
	HRESULT Add_Component();
	void	Update_Situation();
	void	Update_State();
	void	Render_TalkBox(float fTimeDelta);
	void	Show_Collection(float fTimeDelta);

public:
	static CNon_Player_Character* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_RectTexture*	m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom[NPC_PARTS_END];
	CTexture*				m_pTextureCom_Text;
	CTexture*				m_pTextureCom_Item;
	D3DXMATRIX				m_matWorld;

	NPC_TALK				m_ePreTalkState = NPC_TALK_END;
	NPC_TALK				m_eCurTalkState = NPC_TALK_END;

	NPC_STATE				m_ePreState = NPC_STATE_END;
	NPC_STATE				m_eCurState = NPC_STATE_END;

	float					m_fTalkTime = 0.f;
	bool					m_bAbletoTalk = false;
	bool					m_bShowCollection = false;
	bool					m_bClear = true;
};

#define __NON_PLAYER_CHARACTER_H__
#endif // !__NON_PLAYER_CHARACTER_H__
