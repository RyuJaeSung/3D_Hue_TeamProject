#pragma once

#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include "Collision.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Raycast final : public CCollision
{
private:
	explicit CCollision_Raycast(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_Raycast(const CCollision_Raycast& other);
	virtual ~CCollision_Raycast() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override; // ���� ���� �� �ʱ�ȭ
	virtual HRESULT Ready_Component(void* pArg) override; // ������ ���� �� �ʱ�ȭ	

public:
	// ���� �����̽����� �浹 ����
	// ���� : �浹�� ������Ʈ���� �浹 �ִܰŸ� ��ȯ(�������� �ְ� ����)
	//		, ������ ������, ������ ����, ��� ť���� �߽���, ��� ť���� ���� ũ��
	bool IsCollsion_ToCube(float * _pOutDistance, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir,
		const D3DXVECTOR3& _vSrcPos, const D3DXVECTOR3& _vSrcHalfSize);

public:
	static CCollision_Raycast* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component(void* pArg) override; // ������Ÿ�� ����
	virtual void Free() override;
};

END

#endif // !__RAYCAST_H__



