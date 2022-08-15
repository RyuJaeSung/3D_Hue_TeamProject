#include "..\Headers\Transform.h"
#include "PipeLine.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CTransform::CTransform(const CTransform & other)
	: CComponent(other)
	, m_TransformDesc(other.m_TransformDesc)
{
}

const CTransform::TRANSFORM_DESC& CTransform::Get_TransformDesc() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_TransformDesc;
}

D3DXVECTOR3* CTransform::Get_State(D3DXVECTOR3* pOut, STATE_ID eState)
{
	if(nullptr == pOut)
		return nullptr;

	memcpy(pOut, &m_TransformDesc.matWorld.m[eState][0], sizeof(D3DXVECTOR3));
	return pOut;
}

void CTransform::Set_Scale(const D3DXVECTOR3 & In)
{
	m_TransformDesc.vScale = In;
}

void CTransform::Set_StateMatrix(const D3DXMATRIX& mat)
{
	m_TransformDesc.matWorld = mat;
}

void CTransform::Set_TransformDesc(const TRANSFORM_DESC& In)
{
	m_TransformDesc = In;
}

void CTransform::Set_Scale_Down(float fRatio)
{
	m_TransformDesc.vScale *= fRatio;
}

void CTransform::Set_Rotate(float _fRadin, AXIS_TYPE _eRotate)
{
	switch (_eRotate)
	{
	case Engine::CTransform::AXIS_X:
		m_TransformDesc.vRotate.x += _fRadin;
		break;
	case Engine::CTransform::AXIS_Y:
		m_TransformDesc.vRotate.y += _fRadin;
		break;
	case Engine::CTransform::AXIS_Z:
		m_TransformDesc.vRotate.z += _fRadin;
		break;
	default:
		break;
	}
}

const D3DXVECTOR3 & CTransform::Get_Graity()
{
	return m_TransformDesc.vGraity;
}

const D3DXVECTOR3 & CTransform::Get_Velocity()
{
	return m_TransformDesc.vVelocity;
}

void CTransform::Set_Position(const D3DXVECTOR3& In)
{
	m_TransformDesc.vPosition = In;
}
void CTransform::Set_Speed(const float In)
{
	m_TransformDesc.fSpeedPerSec = In;
}
void CTransform::Set_Graity(const float _fGraity)
{
	m_TransformDesc.vGraity = D3DXVECTOR3(0.f, -(_fGraity), 0.f);
}

void CTransform::Set_Pos(float _fPos, AXIS_TYPE _ePosition)
{
	switch (_ePosition)
	{
	case Engine::CTransform::AXIS_X:
		m_TransformDesc.vPosition.x += _fPos;
		break;
	case Engine::CTransform::AXIS_Y:
		m_TransformDesc.vPosition.y += _fPos;
		break;
	case Engine::CTransform::AXIS_Z:
		m_TransformDesc.vPosition.z += _fPos;
		break;
	default:
		break;
	}
}

HRESULT CTransform::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Ready_Component(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORM_DESC));

	//질량을 역질량으로 저장
	m_TransformDesc.fInerseMass = 1.f / m_TransformDesc.fMass;
	return S_OK;
}

HRESULT CTransform::Update_Transform()
{
	if (FAILED(CPipeLine::SetUp_WorldMatrix(&m_TransformDesc.matWorld, m_TransformDesc.vScale, m_TransformDesc.vRotate, m_TransformDesc.vPosition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTransform::Go_Posion(D3DXVECTOR3 pin)
{
	m_TransformDesc.vPosition += pin;

	return S_OK;
}

HRESULT CTransform::Go_Straight(float fTimeDelta)
{
	D3DXVECTOR3 vLook = {};
	Get_State(&vLook, CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	m_TransformDesc.vPosition += vLook * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	return S_OK;
}

HRESULT CTransform::Go_Side(float fTimeDelta)
{
	D3DXVECTOR3 vRight = {};
	Get_State(&vRight, CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);

	m_TransformDesc.vPosition += vRight * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	return S_OK;
}

bool CTransform::Go_ToTarget(D3DXVECTOR3 vTargetPos, float fTimeDelta)
{
	D3DXVECTOR3 vMoveDir = vTargetPos - m_TransformDesc.vPosition;

	float fDistance = D3DXVec3Length(&vMoveDir);
	D3DXVec3Normalize(&vMoveDir, &vMoveDir);

	if (m_TransformDesc.fSpeedPerSec * fTimeDelta >= fDistance)
		return false; // 다 도착했을 때

	m_TransformDesc.vPosition += vMoveDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	return true;
}

HRESULT CTransform::Go_Up(float fTimeDelta)
{
	D3DXVECTOR3 vUp = {};
	Get_State(&vUp, CTransform::STATE_UP);
	D3DXVec3Normalize(&vUp, &vUp);

	m_TransformDesc.vPosition += vUp * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	return S_OK;
}

HRESULT CTransform::Rotate_Axis(AXIS_TYPE eAxis, float fTimeDelta)
{
	switch (eAxis)
	{
	case Engine::CTransform::AXIS_X:
		m_TransformDesc.vRotate.x += m_TransformDesc.fRadianPerSec * fTimeDelta;
		break;
	case Engine::CTransform::AXIS_Y:
		m_TransformDesc.vRotate.y += m_TransformDesc.fRadianPerSec * fTimeDelta;
		break;
	case Engine::CTransform::AXIS_Z:
		m_TransformDesc.vRotate.z += m_TransformDesc.fRadianPerSec * fTimeDelta;
		break;
	}

	return S_OK;
}

HRESULT CTransform::Set_Axis(AXIS_TYPE _eAxis, float _fAngle)
{
	switch (_eAxis)
	{
	case Engine::CTransform::AXIS_X:
		m_TransformDesc.vRotate.x = _fAngle;
		break;
	case Engine::CTransform::AXIS_Y:
		m_TransformDesc.vRotate.y = _fAngle;
		break;
	case Engine::CTransform::AXIS_Z:
		m_TransformDesc.vRotate.z = _fAngle;
		break;
	}

	return S_OK;
}

HRESULT CTransform::Free_Fall(float fTimeDelta)
{
	m_TransformDesc.vVelocity += m_TransformDesc.vGraity * fTimeDelta;
	m_TransformDesc.vPosition += m_TransformDesc.vVelocity;

	return S_OK;
}

HRESULT CTransform::Jump(float fTimeDelta, float _fJumpSpeed)
{
	D3DXVECTOR3 vUp = {};
	Get_State(&vUp, CTransform::STATE_UP);
	D3DXVec3Normalize(&vUp, &vUp);

	m_TransformDesc.vVelocity = vUp * _fJumpSpeed;
	Update_Integrate(fTimeDelta);

	return S_OK;
}

HRESULT CTransform::Go_Gravity()
{
	m_TransformDesc.vPosition += m_TransformDesc.vGraity;
	return S_OK;
}

float CTransform::SpeedPos(SPEED_TYPE _eType)
{
	switch (_eType)
	{
	case Engine::CTransform::SPEED_ADD:
		return 1.f;
	case Engine::CTransform::SPEED_SUB:
		return -1.f;
	case Engine::CTransform::SPEED_END:
	default:
		return 0.f;
	}
}

HRESULT CTransform::Update_Integrate(float _fDeltaTime)
{
	if (m_TransformDesc.fInerseMass <= 0.f)
		return S_OK;

	Update_Pos(_fDeltaTime);
	Update_Speed(_fDeltaTime);

	//누적된 힘 초기화
	ClearForce();
	return S_OK;
}

HRESULT CTransform::Update_Pos(float _fDeltaTime)
{

	m_TransformDesc.vPosition += m_TransformDesc.vVelocity * _fDeltaTime;
	m_TransformDesc.vPosition += m_TransformDesc.vGraity * _fDeltaTime * _fDeltaTime * 0.5f;

	return S_OK;
}

HRESULT CTransform::Update_Speed(float _fDeltaTime)
{

	//힘으로부터 가속도 계산
	D3DXVECTOR3 vResultAcc = m_TransformDesc.vGraity;
	vResultAcc += m_TransformDesc.vForce * m_TransformDesc.fInerseMass;

	//가속도로부터 선형 속도를 업데이트
	m_TransformDesc.vVelocity += vResultAcc * _fDeltaTime;
	//드래그 적용
	m_TransformDesc.vVelocity *= powf(m_TransformDesc.fDamping, _fDeltaTime);

	return S_OK;
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform* pInstance = new CTransform(pDevice);
	
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone_Component(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}

const void CTransform::ClearForce()
{
	ZeroMemory(&m_TransformDesc.vForce, sizeof(D3DXVECTOR3));
}

const void CTransform::ClearVelocity()
{
	ZeroMemory(&m_TransformDesc.vVelocity, sizeof(D3DXVECTOR3));
}

