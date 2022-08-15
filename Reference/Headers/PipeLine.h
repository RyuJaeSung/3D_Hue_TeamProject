#pragma once

#ifndef __PIPELINE_H__

#include "Engine_Include.h"

class CPipeLine final
{
private:
	CPipeLine();
	~CPipeLine();

public:
	static HRESULT SetUp_WorldMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vScale,
		const D3DXVECTOR3& vRotate, /*Radian Table*/
		const D3DXVECTOR3& vPosition);
	static HRESULT SetUp_ViewMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR3& vAt,
		const D3DXVECTOR3& vUp);
	static HRESULT SetUp_ProjectionMatrix(
		D3DXMATRIX* pOut,
		float fFovY,
		float fAspect,
		float fNear,
		float fFar);

private:
	static HRESULT SetUp_TransformMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vRight,		/* 1青 */
		const D3DXVECTOR3& vUp,			/* 2青 */
		const D3DXVECTOR3& vLook,		/* 3青 */
		const D3DXVECTOR3& vPosition	/* 4青 */);
	static HRESULT SetUp_RotationX(
		D3DXVECTOR3* pOut,
		const D3DXVECTOR3* pIn,
		float fRadian);
	static HRESULT SetUp_RotationY(
		D3DXVECTOR3* pOut,
		const D3DXVECTOR3* pIn,
		float fRadian);
	static HRESULT SetUp_RotationZ(
		D3DXVECTOR3* pOut,
		const D3DXVECTOR3* pIn,
		float fRadian);
};

#define __PIPELINE_H__
#endif
