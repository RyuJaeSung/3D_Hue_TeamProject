#pragma once

#ifndef __PLAYER_H__
#include "GameObject.h"
#include "Palette_Circle.h"
#include "Observer.h"

BEGIN(Client)

//���� �ӵ�(float)
#define JUMPSPEED 4.f
//�߷°� ����(float)
#define GRAVITY 5.f

class CPlayer_Hand;
class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer(const CPlayer& other);
	virtual ~CPlayer() = default;

public:
	enum PLAYER_STATE {PLAYER_IDLE, PLAYER_WALK, PLAYER_JUMP, PLAYER_LADDER, PLAYER_DEAD, PLAYER_TALK, PLAYER_END};

public:
	// Getter
	const D3DXVECTOR3*	Get_Pos() { return &m_pTransformCom->Get_TransformDesc().vPosition; }
	const D3DXVECTOR3*	Get_Scale() { return &m_pTransformCom->Get_TransformDesc().vScale; }
	PLAYER_STATE		Get_State() { return m_eCurState; }
	const float			Get_Jump_Speed() { return JUMPSPEED; }
	const float			Get_Speed() { return m_pTransformCom->Get_TransformDesc().fSpeedPerSec; }
	CPalette_Circle*	Get_Palette() { return m_pUserInterface; }

	// Setter
	void Set_Player_State(PLAYER_STATE eCurState) { m_eCurState = eCurState; }
	void Set_StartPos(const D3DXVECTOR3& _vPos);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	void Change_Player_State(float fTimeDelta);

private:
	HRESULT Add_Component();
	HRESULT Movement(float fTimeDelta);
	HRESULT Key_Input(float _fTimeDelta);
	HRESULT SetUp_CollisionDesc();
	
	//�̵��ϸ� �����ð����� ������Ʈ ����
	HRESULT Create_Effect();
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring& LayerTag, const D3DXVECTOR3& vPosition);

	// �÷��̾� �� ����
	HRESULT Create_Player_Hand();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_CubeTexture*	m_pVIBufferCom = nullptr;
	CCollision_CubeSphere*	m_pCubeSphereCollisionCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	CTexture*				m_pTextureCom[PLAYER_END];
	PLAYER_STATE			m_ePreState;			// �÷��̾� �ൿ�� ���� ����� ���� FSM ����
	PLAYER_STATE			m_eCurState;

	float					m_fTextureSpeed = 0.f; // �ִϸ��̼� �ӵ�
	float					m_fTextureIndex = 0.f; // ��� ���ŵǴ� �ؽ��� �ε���
	float					m_fMaxTextureIndex = 0.f; // �ִ� �ؽ��� �ε���

	D3DXVECTOR3				m_vStartPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3				m_vGoalPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	CPalette_Circle*		m_pUserInterface = nullptr;		// �÷��̾ ��ȣ�ۿ��ϴ� UI
	CObserver*				m_pCam_Observer = nullptr;		// ī�޶� ������

	// ź�� ����
	bool					m_IsJump = false;
	float					m_fJumpTime = 0.f;
	float					m_fJumpHeight = 0.f;
	//

	//����Ʈ ����
	// �÷��̾ �̵��ϱ� �����ϸ� �ð��� �缭 �����ð����� ����Ʈ �Լ� Create_Stone_Effect()
	float					m_fEffectTime;
	int						m_iStoneNum;

	// �� ���� ����
	CPlayer_Hand*			m_pPlayerHand = nullptr;
	bool					m_bIsInit = false;
};

END

#define __PLAYER_H__
#endif