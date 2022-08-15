#pragma once

#ifndef __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE_ID
	{
		STATE_RIGHT,
		STATE_UP,
		STATE_LOOK,
		STATE_POSITION
	};

	enum AXIS_TYPE
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z
	};

	enum SPEED_TYPE
	{
		SPEED_ADD,
		SPEED_SUB,
		SPEED_END
	};

public:
	// Desc: Describe(기술하다)의 명사형인 Description의 약자
	typedef struct tagTransformDesc
	{
		tagTransformDesc()
		{
			D3DXMatrixIdentity(&matWorld);
		}

		D3DXVECTOR3 vScale = { 1.f, 1.f, 1.f };
		D3DXVECTOR3 vRotate = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vPosition = { 0.f, 0.f, 0.f };
		D3DXMATRIX	matWorld; // 월드행렬(현재 오브젝트의 상태, 우리 게임의 로컬은 항등이니까)

		D3DXVECTOR3 vVelocity = { 0.f, 0.f, 0.f }; // 속도(방향과 크기값)
		float fSpeedPerSec = 0.f;	// 초당 움직이는 속도
		float fRadianPerSec = 0.f;	// 초당 회전하는 속도

		D3DXVECTOR3 vGraity = { 0.f, 0.f, 0.f };	// 중력	-> Set함수나 y축의 값을 -Graity 값으로 넣어주면 됨!!
		float fDamping = 0.f;	//	Drag 마찰저항값
		float fMass = 0.f;	// 물체의 질량(kg) -> 0이면 무한대의 질량이므로 안됨!
		float fInerseMass = 0.f; //물체의 역질량(kg) -> 역질량이 가속도 구하기 편함(1.f/fMass);
		float fTorque = 0.f;	// 토크 = 비트는 힘
		D3DXVECTOR3	vForce = { 0.f, 0.f, 0.f };	//물체가 받는 힘과 방향

	}TRANSFORM_DESC;

public:
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransform(const CTransform& other);
	virtual ~CTransform() = default;

public:
	// Getter
	const TRANSFORM_DESC& Get_TransformDesc() const;
	D3DXVECTOR3* Get_State(D3DXVECTOR3* pOut, STATE_ID eState);
	const D3DXVECTOR3& Get_Graity();
	const D3DXVECTOR3& Get_Velocity();

	// Setter
	void Set_Scale(const D3DXVECTOR3 & In);
	void Set_StateMatrix(const D3DXMATRIX& mat);
	void Set_TransformDesc(const TRANSFORM_DESC& In);
	void Set_Position(const D3DXVECTOR3& In);
	void Set_Speed(const float In);
	void Set_Graity(const float _fGraity);
	void Set_Pos(float _fPos, AXIS_TYPE _ePosition);
	void Set_Scale_Down(float fRatio);
	void Set_Rotate(float _fRadin, AXIS_TYPE _eRotate);

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg) override;

public:
	HRESULT Update_Transform();
	HRESULT Go_Posion(D3DXVECTOR3 pin);			//이름 확인
	HRESULT Go_Straight(float fTimeDelta);
	HRESULT Go_Side(float fTimeDelta);
	HRESULT Go_Up(float fTimeDelta);
	bool	Go_ToTarget(D3DXVECTOR3 vTargetPos, float fTimeDelta);
	HRESULT Rotate_Axis(AXIS_TYPE eAxis, float fTimeDelta);
	HRESULT Set_Axis(AXIS_TYPE _eAxis, float _fAngle);

public:
	HRESULT Free_Fall(float fTimeDelta);					//자유낙하(중력값 적용)
	HRESULT Jump(float fTimeDelta, float _fJumpSpeed);		//점프
	HRESULT	Go_Gravity();

public:
	float SpeedPos(SPEED_TYPE _eType);	//양수 or 음수 인지 판단 후 매트릭스에 넣을 때 적용시켜줌

public:
	HRESULT Update_Integrate(float _fDeltaTime);		// 통합 업데이트(속도, 위치)
private:
	HRESULT Update_Pos(float _fDeltaTime);				//위치값 업데이트
	HRESULT Update_Speed(float _fDeltaTime);			//속도값 업데이트

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
	const void ClearForce();							//누적 힘 초기화
	//누적 속도 초기화
	const void ClearVelocity();

private:	
	TRANSFORM_DESC	m_TransformDesc;
};

END

#define __TRANSFORM_H__
#endif