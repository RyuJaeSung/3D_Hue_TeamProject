#pragma once

#ifndef __PLAYER_HAND_H__
#include "GameObject.h"
#include "PushBox.h"
BEGIN(Client)
#define PLAYER_HAND_SCALE 0.375f
class CPlayer;
class CPlayer_Hand final : public CGameObject
{
private:
	explicit CPlayer_Hand(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer_Hand(const CPlayer_Hand& other);
	virtual ~CPlayer_Hand() = default;

public:
	enum PLAYER_HAND_STATE {PLAYER_HAND_IDLE, PLAYER_HAND_LADDER_IDLE, PLAYER_HAND_LADDER_GRAP, PLAYER_HAND_BOX_IDLE, PLAYER_HAND_BOX_GRAP, PLAYER_HAND_END};

public:
	// Setter
	void Set_Player_Hand_State(PLAYER_HAND_STATE eCurState) { m_eCurState = eCurState; }
	void Set_Player_Scale(D3DXVECTOR3 _vScale) { m_vPlayerScale = _vScale; }
	void Move_PushBox(float _fMoved);
	void UpMove_When_Ladder() { m_bLadderUpMove = true; }
	void DownMove_When_Ladder() { m_bLadderDownMove = true; }

	// Getter
	PLAYER_HAND_STATE	Get_Player_Hand_State() { return m_ePreState; }
	CPushBox*			Get_Grap_Box() { return m_pPushBox; }

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	void Change_Hand_State();
	HRESULT Add_Component();
	HRESULT SetUp_CollisionDesc();
	BOOL Collision_ToPushBox();
	BOOL Collision_ToLadder();

public:
	static CPlayer_Hand* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_CubeTexture*	m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCollision_CubeSphere*	m_pCubeSphereCollisionCom = nullptr;

	PLAYER_HAND_STATE		m_ePreState = PLAYER_HAND_END;			// �÷��̾� �ൿ�� ���� ����� ���� FSM ����
	PLAYER_HAND_STATE		m_eCurState = PLAYER_HAND_END;

	D3DXVECTOR3				m_vPlayerPos;						// �÷��̾��� Update ���� ��ġ ��
	D3DXVECTOR3				m_vPlayerScale;						// �÷��̾� ���� Ȯ�� �� ��ġ ������ ���� �÷��̾� ������ ��

	D3DXVECTOR3				m_vPos;								// ���� ��ġ ��
	D3DXVECTOR3				m_vScale;							// �� �ؽ��� ������ ���� ������ ����
	D3DXMATRIX				m_matWorld;							// ���� ��ǥ

	CPushBox*				m_pPushBox = nullptr;				// ������ ���� ������ �ּ� ���� ����
	CObserver*				m_pCam_Observer = nullptr;			// ī�޶� ������
	CObserver*				m_pPlayer_Observer = nullptr;		// ���� �޴� �뵵�� ������

	BYTE					m_byTexIndex = 0;
	float					m_fMoveSpeed = 0.f;
	bool					m_bLadderUpMove = false;
	bool					m_bLadderDownMove = false;
	float					m_fLadderHandMove = 0.f;
};

END

#define PLAYER_HAND_H__
#endif // !__PLAYER_HAND_H__
