#pragma once

#ifndef __STATIC_CAMERA_H__

#define CAMERA_DISTANCE 5

#include "Camera.h"

BEGIN(Client)
//선생님의 Static Camera를 베이스로 만듦
class CStatic_Camera final : public CCamera
{
public:
	enum CAMERA_OUTLINE { CAMERA_LT, CAMERA_RT, CAMERA_LB, CAMERA_RB, CAMERA_END};

private:
	explicit CStatic_Camera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStatic_Camera(const CStatic_Camera& _other);
	virtual ~CStatic_Camera() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * _pArg) override;
	virtual int Update_GameObject(float _fTimeDelta) override;
	virtual int LateUpdate_GameObject(float _fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	//Get
	const CAMERA_EFFECT& Get_CameraEffect() { return m_eCurEffect; }

	//Set
	const HRESULT Set_Cam_Effect(CAMERA_EFFECT _eEffect);
	const void Set_Cam_OutLine(D3DXVECTOR3 _pTilePos[]);

public:
	int Change_Camera_View(CAMERA_VIEW _eView, float _fTimeDelta);																		// 카메라 시점 변경 함수
	int Change_Camera_Effect(CAMERA_EFFECT _eEffect, float _fTimeDelta, D3DXVECTOR3 _vTarget = { 0.f,0.f,0.f });						// 카메라 이벤트를 위한 함수
private:
	HRESULT Movement(float _fTimeDelta);

	int First_Person_View(float _fTimeDelta);
	int Third_Person_View(float _fTimeDelta);

	int Camera_Shaking(D3DXVECTOR3 _vPlayerPos, float _fFrequency,  float _fTimeDelta);
	int Camera_Revolution_Y(D3DXVECTOR3 _vPlayerPos, D3DXVECTOR3 _vTargetPos, float _fTimeDelta);
	int Camera_Target_Zoom(D3DXVECTOR3 _vPlayerPos, D3DXVECTOR3 _vTargetPos, float _fTimeDelta);
	int Camera_Explorer(float _fTimeDelta);

public:
	static CStatic_Camera* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	float			m_fDistance = 0.f;	// 카메라와 대상과의 거리.
	float			m_fCamRadian = 0.f; // 카메라가 바라보는 각도.

	CAMERA_VIEW		m_ePreView;			// FSM 패턴 - 시점 관리
	CAMERA_VIEW		m_eCurView;

	//1인칭 시점시 사용할 변수
	RECT			m_rcWindow;						// 게임 화면 사각형
	POINT			m_ptCenter;						// 게임 화면의 중앙
	D3DVIEWPORT9	m_vpViewPort;					// 뷰포트
	float			m_fHorizon_Sensitive = 0.f;		// 1인칭시 상하 감도 조절용
	float			m_fVertical_Sensitive = 0.f;	// 1인칭시 좌우 감도 조절용

	//이펙트 처리를 위한 변수
	CAMERA_EFFECT	m_ePreEffect;		// FSM 패턴 - 이벤트 관리
	CAMERA_EFFECT	m_eCurEffect;

	float			m_fShakingFrequency = 0.f;			// 카메라 진동 효과 시, 진동의 세기
	float			m_fRevolution_Distance = 0.f;		// 카메라 공전 효과 시, 회전 반지름의 거리
	float			m_fRevolution_Round = 0.f;			// 카메라 공전 효과 시, 1회 회전을 적용하기 위해 필요한 값(= 이펙트가 적용되는 시간)
	float			m_fRevolution_Count = 0.f;			// 카메라 공전 효과 시, 회전 횟수

	float			m_fEffectTime = 0.f;

	// 카메라 락을 위한 변수
	D3DXVECTOR3		m_ArrCameraOutLine[CAMERA_END];
};

END

#define __STATIC_CAMERA_H__
#endif