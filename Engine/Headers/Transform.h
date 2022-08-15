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
	typedef struct tagTransformDesc
	{
		tagTransformDesc()
		{
			D3DXMatrixIdentity(&matWorld);
		}

		D3DXVECTOR3 vScale = { 1.f, 1.f, 1.f };
		D3DXVECTOR3 vRotate = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vPosition = { 0.f, 0.f, 0.f };
		D3DXMATRIX	matWorld; 

		D3DXVECTOR3 vVelocity = { 0.f, 0.f, 0.f };
		float fSpeedPerSec = 0.f;	
		float fRadianPerSec = 0.f;	

		D3DXVECTOR3 vGraity = { 0.f, 0.f, 0.f };	
		float fDamping = 0.f;	
		float fMass = 0.f;	
		float fInerseMass = 0.f; 
		float fTorque = 0.f;	
		D3DXVECTOR3	vForce = { 0.f, 0.f, 0.f };	

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
	HRESULT Go_Posion(D3DXVECTOR3 pin);			//�̸� Ȯ��
	HRESULT Go_Straight(float fTimeDelta);
	HRESULT Go_Side(float fTimeDelta);
	HRESULT Go_Up(float fTimeDelta);
	bool	Go_ToTarget(D3DXVECTOR3 vTargetPos, float fTimeDelta);
	HRESULT Rotate_Axis(AXIS_TYPE eAxis, float fTimeDelta);
	HRESULT Set_Axis(AXIS_TYPE _eAxis, float _fAngle);

public:
	HRESULT Free_Fall(float fTimeDelta);					//��������(�߷°� ����)
	HRESULT Jump(float fTimeDelta, float _fJumpSpeed);		//����
	HRESULT	Go_Gravity();

public:
	float SpeedPos(SPEED_TYPE _eType);	//��� or ���� ���� �Ǵ� �� ��Ʈ������ ���� �� ���������

public:
	HRESULT Update_Integrate(float _fDeltaTime);		// ���� ������Ʈ(�ӵ�, ��ġ)
private:
	HRESULT Update_Pos(float _fDeltaTime);				//��ġ�� ������Ʈ
	HRESULT Update_Speed(float _fDeltaTime);			//�ӵ��� ������Ʈ

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
	const void ClearForce();							//���� �� �ʱ�ȭ
	//���� �ӵ� �ʱ�ȭ
	const void ClearVelocity();

private:	
	TRANSFORM_DESC	m_TransformDesc;
};

END

#define __TRANSFORM_H__
#endif