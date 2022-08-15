#include "stdafx.h"
#include "..\Headers\Static_Camera.h"
#include "Player.h"
#include "Camera_Observer.h"

USING(Client)

CStatic_Camera::CStatic_Camera(LPDIRECT3DDEVICE9 _pDevice)
	: CCamera(_pDevice)
{
}

CStatic_Camera::CStatic_Camera(const CStatic_Camera & _other)
	: CCamera(_other)
{
}

HRESULT CStatic_Camera::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CStatic_Camera::Ready_GameObject(void * _pArg)
{
	if (FAILED(CCamera::Ready_GameObject(_pArg)))
		return E_FAIL;

	m_ePreEffect = CAM_EFFECT_NORMAL;
	m_eCurEffect = CAM_EFFECT_NORMAL;

	m_ePreView = CAM_VIEW_THIRD_SIDE;
	m_eCurView = CAM_VIEW_THIRD_SIDE;

	m_fDistance = 5.f;
	m_fShakingFrequency = 10.f;
	m_fRevolution_Distance = 5.f;
	m_fRevolution_Count = 2.f;
	m_fHorizon_Sensitive = 0.25f;
	m_fVertical_Sensitive = 0.5;

	// 1��Ī ī�޶� ���콺 �̵��� ���� �ʱ�ȭ ���� ����

	GetClientRect(g_hWnd, &m_rcWindow);

	// ȭ�� ������ �� ������ �Ѵ�.
	POINT ptBegin, ptEnd;
	ptBegin.x = m_rcWindow.left;
	ptBegin.y = m_rcWindow.top;
	ptEnd.x = m_rcWindow.right;
	ptEnd.y = m_rcWindow.bottom;

	ClientToScreen(g_hWnd, &ptBegin);
	ClientToScreen(g_hWnd, &ptEnd);

	m_rcWindow.left = ptBegin.x;
	m_rcWindow.top = ptBegin.y;
	m_rcWindow.right = ptEnd.x;
	m_rcWindow.bottom = ptEnd.y;

	//WINCX, WINCY �������� �ϸ� �»�� ������ ���õ�.
	m_ptCenter.x = (ptBegin.x + ptEnd.x) / 2;
	m_ptCenter.y = (ptBegin.y + ptEnd.y) / 2;

	// ���콺�� �߾����� ������Ų��.
	SetCursorPos(m_ptCenter.x, m_ptCenter.y);

	// ����Ʈ ����
	m_vpViewPort.X = 0;
	m_vpViewPort.Y = 0;
	m_vpViewPort.Width = m_rcWindow.right - m_rcWindow.left;
	m_vpViewPort.Height = m_rcWindow.bottom - m_rcWindow.top;
	m_vpViewPort.MinZ = 0.f;
	m_vpViewPort.MaxZ = 1.0;
	// 1��Ī ī�޶� ���콺 �̵��� ���� �ʱ�ȭ ���� ��

	CManagement::Get_Instance()->Notify(OBJ_DATA::CAM_VIEW, &m_ePreView);
	CManagement::Get_Instance()->Notify(OBJ_DATA::CAM_EFFECT, &m_ePreEffect);
	return S_OK;
}

int CStatic_Camera::Update_GameObject(float _fTimeDelta)
{
	if (FAILED(Movement(_fTimeDelta)))
		return 0;
	return 0;
}

int CStatic_Camera::LateUpdate_GameObject(float _fTimeDelta)
{
	CCamera::LateUpdate_GameObject(_fTimeDelta);

	return 0;
}

HRESULT CStatic_Camera::Render_GameObject()
{
	return S_OK;
}

const HRESULT CStatic_Camera::Set_Cam_Effect(CAMERA_EFFECT _eEffect)
{
	if (_eEffect >= CAM_EFFECT_END)
		return E_FAIL;
	m_eCurEffect = _eEffect;

	return S_OK;
}

const void CStatic_Camera::Set_Cam_OutLine(D3DXVECTOR3 _pTilePos[])
{
	for (int i = 0; i < CAMERA_END; ++i)
	{
		m_ArrCameraOutLine[i] = _pTilePos[i];
	}
}

int CStatic_Camera::Change_Camera_View(CAMERA_VIEW _eView, float _fTimeDelta)
{
	m_eCurView = _eView;
	if (m_ePreView == m_eCurView)
		return 0;		// �ƹ� �ϵ� �Ͼ�� �ʴ´�.
	else
	{
		switch (m_eCurView)
		{
		case CAM_VIEW_FIRST_PERSON:
			First_Person_View(_fTimeDelta);
			break;
		case CAM_VIEW_THIRD_SIDE:
			Third_Person_View(_fTimeDelta);
			break;
		}
		m_ePreView = m_eCurView;
		CManagement::Get_Instance()->Notify(OBJ_DATA::CAM_VIEW, &m_ePreView);
	}
	return 0;
}

int CStatic_Camera::Change_Camera_Effect(CAMERA_EFFECT _eEffect, float _fTimeDelta, D3DXVECTOR3 _vTarget)
{
	if (0.f < m_fEffectTime && _eEffect != CAM_EFFECT_REVOLUTION_Y)
		return 0;

	m_eCurEffect = _eEffect;
	if (m_ePreEffect == m_eCurEffect)
		return 0;		// �ƹ� �ϵ� �Ͼ�� �ʴ´�.
	else
	{
		switch (m_eCurEffect)
		{
		case CAM_EFFECT_NORMAL:
			m_fEffectTime = 0.f;
			break;
		case CAM_EFFECT_SHAKING:
			m_fEffectTime = 0.5f;
			break;
		case CAM_EFFECT_REVOLUTION_Y:
			if (D3DXVECTOR3(0.f, 0.f, 0.f) == _vTarget)
				return 0;
			m_fEffectTime = 3.f;
			m_fRevolution_Round = m_fEffectTime;
			break;
		case CAM_EFFECT_TARGET_ZOOM:
			if (D3DXVECTOR3(0.f, 0.f, 0.f) == _vTarget)
				return 0;
			m_fEffectTime = 4.f;
			break;
		case CAM_EFFECT_EXPLORER:
			m_fEffectTime = 4.f;
			break;
		}
		m_ePreEffect = m_eCurEffect;
		CManagement::Get_Instance()->Notify(OBJ_DATA::CAM_EFFECT, &m_ePreEffect);
	}
	return 0;
}

HRESULT CStatic_Camera::Movement(float _fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	if (CPlayer::PLAYER_TALK == dynamic_cast<CPlayer*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player")))->Get_State())
	{
		m_fDistance = 10.f;
		m_CameraDesc.vEye.y = 5.f;
		m_CameraDesc.vEye.z = -4.f;
		m_CameraDesc.vAt.y = 4.f;
		return S_OK;
	}
	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	pManagement->Set_OrthoMatrix(m_ptCenter.x * 2.f, m_ptCenter.y * 2.f);

	D3DXVECTOR3 vPlayerRight, vPlayerLook, vPlayerPos;
	// �÷��̾��� Right, Look, Pos ���� ����
	pPlayerTransform->Get_State(&vPlayerRight, CTransform::STATE_RIGHT);
	pPlayerTransform->Get_State(&vPlayerLook, CTransform::STATE_LOOK);
	pPlayerTransform->Get_State(&vPlayerPos, CTransform::STATE_POSITION);

	// ���� ����(���� ����)�� ������
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	// right ���� �������� ī�޶� ȸ��
	D3DXMATRIX matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, m_fCamRadian); // ������ �� ȸ��
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);	// �������ͷ� Look ���
	switch (m_ePreView)
	{
	case CAM_VIEW_FIRST_PERSON:
		vPlayerLook *= m_fDistance; // �� ������ ���� ����
		m_CameraDesc.vEye = vPlayerPos;
		m_CameraDesc.vAt = vPlayerPos + vPlayerLook;
		break;
	case CAM_VIEW_THIRD_SIDE:
		vPlayerLook *= -m_fDistance;		// �Ÿ��� ������ŭ Look�� ����

		float y = pPlayerTransform->Get_TransformDesc().vScale.y;

		float CameraLimitX = 4.3f;			// ��ũ�� ��
		float CameraLimitY = 2.5f;			// ��ũ�� ��

											// X��
		if (vPlayerPos.x <= m_ArrCameraOutLine[CAMERA_LT].x + CameraLimitX)
			vPlayerPos.x = m_ArrCameraOutLine[CAMERA_LT].x + CameraLimitX;
		else if (vPlayerPos.x >= m_ArrCameraOutLine[CAMERA_RT].x - CameraLimitX)
			vPlayerPos.x = m_ArrCameraOutLine[CAMERA_RT].x - CameraLimitX;

		// Y��
		if (vPlayerPos.y <= m_ArrCameraOutLine[CAMERA_LB].y + CameraLimitY)
			vPlayerPos.y = m_ArrCameraOutLine[CAMERA_LB].y + CameraLimitY;
		else if (vPlayerPos.y >= m_ArrCameraOutLine[CAMERA_LT].y - CameraLimitY)
			vPlayerPos.y = m_ArrCameraOutLine[CAMERA_LT].y - CameraLimitY;

		m_CameraDesc.vEye = vPlayerLook + vPlayerPos;

		m_CameraDesc.vAt = vPlayerPos;
		break;
	}


	if (m_eCurEffect != m_ePreEffect)
		Change_Camera_Effect(m_eCurEffect, _fTimeDelta);

	switch (m_ePreEffect)
	{
	case CAM_EFFECT_SHAKING:
		Camera_Shaking(vPlayerPos, m_fShakingFrequency, _fTimeDelta);
		break;
	case CAM_EFFECT_REVOLUTION_Y:
		Camera_Revolution_Y(vPlayerPos, vPlayerPos, _fTimeDelta);
		break;
	case CAM_EFFECT_TARGET_ZOOM:
		break;
	case CAM_EFFECT_EXPLORER:
		Camera_Explorer(_fTimeDelta);
		break;
	}

	if (CAM_EFFECT_REVOLUTION_Y == m_ePreEffect
		|| CAM_EFFECT_EXPLORER == m_ePreEffect)
		return 0;

	if (pManagement->KeyPressing(VK_ADD))
		m_fDistance -= 10.f * _fTimeDelta;
	if (pManagement->KeyPressing(VK_SUBTRACT))
		m_fDistance += 10.f * _fTimeDelta;
	if (pManagement->KeyPressing(VK_NUMPAD8) && CAM_VIEW_FIRST_PERSON == m_ePreView)
		m_fCamRadian += D3DXToRadian(45.f) * _fTimeDelta;
	if (pManagement->KeyPressing(VK_NUMPAD2) && CAM_VIEW_FIRST_PERSON == m_ePreView)
		m_fCamRadian -= D3DXToRadian(45.f) * _fTimeDelta;
#ifdef _DEBUG
	if (pManagement->KeyPressing(VK_NUMPAD8) && CAM_VIEW_THIRD_SIDE == m_ePreView)
		m_fCamRadian += D3DXToRadian(45.f) * _fTimeDelta;
	if (pManagement->KeyPressing(VK_NUMPAD2) && CAM_VIEW_THIRD_SIDE == m_ePreView)
		m_fCamRadian -= D3DXToRadian(45.f) * _fTimeDelta;
#endif // _DEBUG

	// ���콺 ���� �̵��� ����
	if (CAM_VIEW_FIRST_PERSON == m_ePreView)
	{
		POINT ptCurrent;
		GetCursorPos(&ptCurrent);
		SetCursorPos(m_ptCenter.x, m_ptCenter.y);
		if (ptCurrent.y < m_ptCenter.y) // mouse up look
		{
			m_fCamRadian -= D3DXToRadian(45.f) * _fTimeDelta * abs(ptCurrent.y - m_ptCenter.y) * m_fVertical_Sensitive;
			// ������ ���ݱ����� ȸ��. �� ī�޶� ������ 90�� �̻����� �Ѿ�� �ʵ��� ��.
			if (m_fCamRadian < (-PI / 2.f))
				m_fCamRadian = (-PI / 2.f);
		}
		if (ptCurrent.y > m_ptCenter.y) // mouse down look
		{
			m_fCamRadian += D3DXToRadian(45.f) * _fTimeDelta * abs(ptCurrent.y - m_ptCenter.y) * m_fVertical_Sensitive;
			if (m_fCamRadian >(PI / 2.f))
				m_fCamRadian = (PI / 2.f);
		}

		// �÷��̾��� ������� ����Ǵ� �� ����.
		if (ptCurrent.x < m_ptCenter.x) // mouse left look
			pPlayerTransform->Rotate_Axis(CTransform::AXIS_Y, -_fTimeDelta * abs(ptCurrent.x - m_ptCenter.x) * m_fHorizon_Sensitive);

		if (ptCurrent.x > m_ptCenter.x) // mouse right look
			pPlayerTransform->Rotate_Axis(CTransform::AXIS_Y, _fTimeDelta * abs(ptCurrent.x - m_ptCenter.x) * m_fHorizon_Sensitive);
	}

	// ���� ������ ���� Ű ����
	if (pManagement->KeyDown(VK_NUMPAD5))
	{
		switch (m_ePreView)
		{
		case CAM_VIEW_FIRST_PERSON:
			Change_Camera_View(CAM_VIEW_THIRD_SIDE, _fTimeDelta);
			break;
		case CAM_VIEW_THIRD_SIDE:
			Change_Camera_View(CAM_VIEW_FIRST_PERSON, _fTimeDelta);
			break;
		}
	}

	return S_OK;
}

#pragma region CAMERA_VIEW

int CStatic_Camera::First_Person_View(float _fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 1;
	CTransform* pPlayer_Transform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player"), __T("Com_Transform"));
	if (nullptr == pPlayer_Transform)
		return 2;
	pPlayer_Transform->Set_Scale(D3DXVECTOR3(abs(pPlayer_Transform->Get_TransformDesc().vScale.x), pPlayer_Transform->Get_TransformDesc().vScale.y, pPlayer_Transform->Get_TransformDesc().vScale.z));
	pPlayer_Transform->Set_Axis(CTransform::AXIS_Y, D3DXToRadian(90.f));	// �÷��̾ �ٶ󺸴� ���� ����. ����������� ���� �� ����.
	m_fCamRadian = D3DXToRadian(0.f);

	// 1��Ī�� ��� ������� ���̴� ���� �ڿ�������
	m_CameraDesc.fNear = 0.1f;
	// 3��Ī�� ���� ���콺 ��ġ ���� �ٷ� �����ϴ� ���� ����
	SetCursorPos(m_ptCenter.x, m_ptCenter.y);
	return 0;

}

int CStatic_Camera::Third_Person_View(float _fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 1;
	CTransform* pPlayer_Transform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player"), __T("Com_Transform"));
	if (nullptr == pPlayer_Transform)
		return 2;

	pPlayer_Transform->Set_Axis(CTransform::AXIS_Y, 0.f);
	m_fCamRadian = D3DXToRadian(0.f);
	m_CameraDesc.fNear = 1.f;
	return 0;
}

#pragma endregion

#pragma region CAMERA_EFFECT

int CStatic_Camera::Camera_Shaking(D3DXVECTOR3 _vPlayerPos, float _fFrequency, float _fTimeDelta)
{
	m_fEffectTime -= _fTimeDelta;
	float fAngle = m_fEffectTime;

	// ���Ϸθ� ����
	m_CameraDesc.vEye.y = _vPlayerPos.y + cosf(_fFrequency * 10.f * fAngle) * m_fEffectTime / 5.f; //* powf(1.f, fAngle);
	m_CameraDesc.vAt.y += cosf(_fFrequency * 10.f * fAngle) * m_fEffectTime / 5.f; //sinf(20.f * fAngle) * powf(0.5f, fAngle);

	if (m_fEffectTime < 0.f)
		Change_Camera_Effect(CAM_EFFECT_NORMAL, _fTimeDelta);

	return 0;
}

int CStatic_Camera::Camera_Revolution_Y(D3DXVECTOR3 _vPlayerPos, D3DXVECTOR3 _vTargetPos, float _fTimeDelta)
{
	m_fEffectTime -= _fTimeDelta;
	float fSpeed = m_fEffectTime / m_fRevolution_Round;
	float fAngle = PI * 2.f * fSpeed * fSpeed * m_fRevolution_Count;

	m_CameraDesc.vEye = _vTargetPos;
	m_CameraDesc.vEye.x += m_fRevolution_Distance * sinf(fAngle);
	m_CameraDesc.vEye.z += m_fRevolution_Distance * -cosf(fAngle);
	m_CameraDesc.vAt = _vTargetPos;

	if (m_fEffectTime < 0.f)
		Change_Camera_Effect(CAM_EFFECT_NORMAL, _fTimeDelta);

	return 0;
}

int CStatic_Camera::Camera_Target_Zoom(D3DXVECTOR3 _vPlayerPos, D3DXVECTOR3 _vTargetPos, float _fTimeDelta)
{
	m_fEffectTime -= _fTimeDelta;
	float fAngle = m_fEffectTime * 90.f;

	if (m_fEffectTime < 0.f)
		Change_Camera_Effect(CAM_EFFECT_NORMAL, _fTimeDelta);

	return 0;
}

int CStatic_Camera::Camera_Explorer(float _fTimeDelta)
{
	m_fEffectTime -= _fTimeDelta;

	if (2.f < m_fEffectTime)
	{
		D3DXVECTOR3 vStage_Center = { 0.f, 0.f, 0.f };

		vStage_Center.x = m_ArrCameraOutLine[CAMERA_RT].x / 2.f;
		vStage_Center.y = m_ArrCameraOutLine[CAMERA_RT].y / 2.f;
		m_CameraDesc.vEye = vStage_Center;
		m_CameraDesc.vEye.z -= m_fDistance;

		m_CameraDesc.vAt = vStage_Center;

		m_fDistance += 8.f * _fTimeDelta;
	}
	else
	{
		// �÷��̾� ���� �ٶ󺸵��� �ޱ� ���� ������� ��
		D3DXVECTOR3 vStage_Center = { 0.f, 0.f, 0.f };

		vStage_Center.x = m_ArrCameraOutLine[CAMERA_RT].x / 2.f;
		vStage_Center.y = m_ArrCameraOutLine[CAMERA_RT].y / 2.f;

		m_CameraDesc.vEye = vStage_Center;
		m_CameraDesc.vEye.z -= m_fDistance;

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		CTransform* pPlayer_Transform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
		if (nullptr == pPlayer_Transform)
			return 0;

		D3DXVECTOR3 vPlayer_Pos = pPlayer_Transform->Get_TransformDesc().vPosition;

		if (vStage_Center.x < vPlayer_Pos.x) // �÷��̾ �������� ���ʿ� �ִ�.
			m_CameraDesc.vEye.x = vPlayer_Pos.x - 5.f;
		else
			m_CameraDesc.vEye.x = vPlayer_Pos.x + 5.f;

		if (SCENE_STAGE7 == pManagement->Get_Current_SceneID())
			m_CameraDesc.vEye.y = vPlayer_Pos.y - 5.f;

		m_CameraDesc.vAt = vPlayer_Pos;


		m_fDistance -= 8.f * _fTimeDelta;
	}



	if (m_fEffectTime < 0.f)
		Change_Camera_Effect(CAM_EFFECT_NORMAL, _fTimeDelta);

	return 0;
}

#pragma endregion

CStatic_Camera * CStatic_Camera::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CStatic_Camera* pInstance = new CStatic_Camera(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CStatic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStatic_Camera::Clone_GameObject(void * _pArg)
{
	CStatic_Camera* pInstance = new CStatic_Camera(*this); // ���� ����

	if (FAILED(pInstance->Ready_GameObject(_pArg)))
	{
		ERR_MSG(L"Failed To Clone CStatic_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatic_Camera::Free()
{
	CCamera::Free();
}
