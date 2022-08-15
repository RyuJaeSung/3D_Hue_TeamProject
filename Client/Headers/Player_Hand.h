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
	// CGameObject을(를) 통해 상속됨
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

	PLAYER_HAND_STATE		m_ePreState = PLAYER_HAND_END;			// 플레이어 행동에 따른 모션을 위한 FSM 패턴
	PLAYER_HAND_STATE		m_eCurState = PLAYER_HAND_END;

	D3DXVECTOR3				m_vPlayerPos;						// 플레이어의 Update 이후 위치 값
	D3DXVECTOR3				m_vPlayerScale;						// 플레이어 방향 확인 및 위치 설정을 위한 플레이어 스케일 값

	D3DXVECTOR3				m_vPos;								// 손의 위치 값
	D3DXVECTOR3				m_vScale;							// 손 텍스쳐 반전을 위한 스케일 저장
	D3DXMATRIX				m_matWorld;							// 월드 좌표

	CPushBox*				m_pPushBox = nullptr;				// 손으로 잡은 상자의 주소 값을 보관
	CObserver*				m_pCam_Observer = nullptr;			// 카메라 옵저버
	CObserver*				m_pPlayer_Observer = nullptr;		// 색상 받는 용도의 옵저버

	BYTE					m_byTexIndex = 0;
	float					m_fMoveSpeed = 0.f;
	bool					m_bLadderUpMove = false;
	bool					m_bLadderDownMove = false;
	float					m_fLadderHandMove = 0.f;
};

END

#define PLAYER_HAND_H__
#endif // !__PLAYER_HAND_H__
