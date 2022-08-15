#pragma once

#ifndef __PLAYER_H__
#include "GameObject.h"
#include "Palette_Circle.h"
#include "Observer.h"

BEGIN(Client)

//점프 속도(float)
#define JUMPSPEED 4.f
//중력값 적용(float)
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
	// CGameObject을(를) 통해 상속됨
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
	
	//이동하면 일정시간마다 돌이펙트 생성
	HRESULT Create_Effect();
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring& LayerTag, const D3DXVECTOR3& vPosition);

	// 플레이어 손 생성
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
	PLAYER_STATE			m_ePreState;			// 플레이어 행동에 따른 모션을 위한 FSM 패턴
	PLAYER_STATE			m_eCurState;

	float					m_fTextureSpeed = 0.f; // 애니메이션 속도
	float					m_fTextureIndex = 0.f; // 계속 갱신되는 텍스쳐 인덱스
	float					m_fMaxTextureIndex = 0.f; // 최대 텍스쳐 인덱스

	D3DXVECTOR3				m_vStartPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3				m_vGoalPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	CPalette_Circle*		m_pUserInterface = nullptr;		// 플레이어가 상호작용하는 UI
	CObserver*				m_pCam_Observer = nullptr;		// 카메라 옵저버

	// 탄성 관련
	bool					m_IsJump = false;
	float					m_fJumpTime = 0.f;
	float					m_fJumpHeight = 0.f;
	//

	//이펙트 변수
	// 플레이어가 이동하기 시작하면 시간을 재서 일정시간마다 이펙트 함수 Create_Stone_Effect()
	float					m_fEffectTime;
	int						m_iStoneNum;

	// 손 관련 변수
	CPlayer_Hand*			m_pPlayerHand = nullptr;
	bool					m_bIsInit = false;
};

END

#define __PLAYER_H__
#endif