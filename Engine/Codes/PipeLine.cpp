#include "..\Headers\PipeLine.h"



CPipeLine::CPipeLine()
{
}


CPipeLine::~CPipeLine()
{
}

HRESULT CPipeLine::SetUp_WorldMatrix(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3 & vScale, 
	const D3DXVECTOR3 & vRotate, 
	const D3DXVECTOR3 & vPosition)
{
	if (nullptr == pOut)
		return E_FAIL;

	D3DXVECTOR3 vRight	= { 1.f, 0.f, 0.f };
	D3DXVECTOR3 vUp		= { 0.f, 1.f, 0.f };
	D3DXVECTOR3 vLook	= { 0.f, 0.f, 1.f };	

	// 스케일 세팅.
	vRight.x *= vScale.x;
	vUp.y *= vScale.y;
	vLook.z *= vScale.z;

	// 회전(자전) 세팅.
	// x축 회전
	SetUp_RotationX(&vRight, &vRight, vRotate.x);
	SetUp_RotationX(&vUp, &vUp, vRotate.x);
	SetUp_RotationX(&vLook, &vLook, vRotate.x);

	// y축 회전
	SetUp_RotationY(&vRight, &vRight, vRotate.y);
	SetUp_RotationY(&vUp, &vUp, vRotate.y);
	SetUp_RotationY(&vLook, &vLook, vRotate.y);

	// z축 회전
	SetUp_RotationZ(&vRight, &vRight, vRotate.z);
	SetUp_RotationZ(&vUp, &vUp, vRotate.z);
	SetUp_RotationZ(&vLook, &vLook, vRotate.z);

	if (FAILED(SetUp_TransformMatrix(pOut, vRight, vUp, vLook, vPosition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPipeLine::SetUp_ViewMatrix(
	D3DXMATRIX * pOut, 
	const D3DXVECTOR3 & vEye, 
	const D3DXVECTOR3 & vAt, 
	const D3DXVECTOR3 & vUp)
{
	return E_NOTIMPL;
}

HRESULT CPipeLine::SetUp_ProjectionMatrix(
	D3DXMATRIX * pOut, 
	float fFovY, 
	float fAspect, 
	float fNear, 
	float fFar)
{
	return E_NOTIMPL;
}

HRESULT CPipeLine::SetUp_TransformMatrix(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3 & vRight, 
	const D3DXVECTOR3 & vUp, 
	const D3DXVECTOR3 & vLook, 
	const D3DXVECTOR3 & vPosition)
{
	if (nullptr == pOut)
		return E_FAIL;

	D3DXMatrixIdentity(pOut);

	memcpy(&pOut->m[0][0], &vRight, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[1][0], &vUp, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[2][0], &vLook, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[3][0], &vPosition, sizeof(D3DXVECTOR3));

	return S_OK;
}

HRESULT CPipeLine::SetUp_RotationX(
	D3DXVECTOR3 * pOut, 
	const D3DXVECTOR3 * pIn, 
	float fRadian)
{
	D3DXVECTOR3 vTemp = *pIn;

	// X축 회전
	pOut->y = vTemp.y * cosf(fRadian) - vTemp.z * sinf(fRadian);
	pOut->z = vTemp.y * sinf(fRadian) + vTemp.z * cosf(fRadian);

	return S_OK;
}

HRESULT CPipeLine::SetUp_RotationY(
	D3DXVECTOR3 * pOut, 
	const D3DXVECTOR3 * pIn, 
	float fRadian)
{
	D3DXVECTOR3 vTemp = *pIn;

	// Y축 회전
	pOut->x = vTemp.x * cosf(fRadian) + vTemp.z * sinf(fRadian);
	pOut->z = vTemp.x * -sinf(fRadian) + vTemp.z * cosf(fRadian);

	return S_OK;
}

HRESULT CPipeLine::SetUp_RotationZ(
	D3DXVECTOR3 * pOut, 
	const D3DXVECTOR3 * pIn, 
	float fRadian)
{
	D3DXVECTOR3 vTemp = *pIn;

	// z축 회전
	pOut->x = vTemp.x * cosf(fRadian) - vTemp.y * sinf(fRadian);
	pOut->y = vTemp.x * sinf(fRadian) + vTemp.y * cosf(fRadian);

	return S_OK;
}
