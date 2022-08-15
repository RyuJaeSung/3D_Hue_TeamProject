#pragma once

#ifndef __STATIC_CAMERA_H__

#define CAMERA_DISTANCE 5

#include "Camera.h"

BEGIN(Client)
//�������� Static Camera�� ���̽��� ����
class CStatic_Camera final : public CCamera
{
public:
	enum CAMERA_OUTLINE { CAMERA_LT, CAMERA_RT, CAMERA_LB, CAMERA_RB, CAMERA_END};

private:
	explicit CStatic_Camera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStatic_Camera(const CStatic_Camera& _other);
	virtual ~CStatic_Camera() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
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
	int Change_Camera_View(CAMERA_VIEW _eView, float _fTimeDelta);																		// ī�޶� ���� ���� �Լ�
	int Change_Camera_Effect(CAMERA_EFFECT _eEffect, float _fTimeDelta, D3DXVECTOR3 _vTarget = { 0.f,0.f,0.f });						// ī�޶� �̺�Ʈ�� ���� �Լ�
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
	float			m_fDistance = 0.f;	// ī�޶�� ������ �Ÿ�.
	float			m_fCamRadian = 0.f; // ī�޶� �ٶ󺸴� ����.

	CAMERA_VIEW		m_ePreView;			// FSM ���� - ���� ����
	CAMERA_VIEW		m_eCurView;

	//1��Ī ������ ����� ����
	RECT			m_rcWindow;						// ���� ȭ�� �簢��
	POINT			m_ptCenter;						// ���� ȭ���� �߾�
	D3DVIEWPORT9	m_vpViewPort;					// ����Ʈ
	float			m_fHorizon_Sensitive = 0.f;		// 1��Ī�� ���� ���� ������
	float			m_fVertical_Sensitive = 0.f;	// 1��Ī�� �¿� ���� ������

	//����Ʈ ó���� ���� ����
	CAMERA_EFFECT	m_ePreEffect;		// FSM ���� - �̺�Ʈ ����
	CAMERA_EFFECT	m_eCurEffect;

	float			m_fShakingFrequency = 0.f;			// ī�޶� ���� ȿ�� ��, ������ ����
	float			m_fRevolution_Distance = 0.f;		// ī�޶� ���� ȿ�� ��, ȸ�� �������� �Ÿ�
	float			m_fRevolution_Round = 0.f;			// ī�޶� ���� ȿ�� ��, 1ȸ ȸ���� �����ϱ� ���� �ʿ��� ��(= ����Ʈ�� ����Ǵ� �ð�)
	float			m_fRevolution_Count = 0.f;			// ī�޶� ���� ȿ�� ��, ȸ�� Ƚ��

	float			m_fEffectTime = 0.f;

	// ī�޶� ���� ���� ����
	D3DXVECTOR3		m_ArrCameraOutLine[CAMERA_END];
};

END

#define __STATIC_CAMERA_H__
#endif