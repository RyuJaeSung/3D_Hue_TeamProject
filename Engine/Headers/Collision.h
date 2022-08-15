#ifndef __COLLISION_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision abstract : public CComponent
{
protected:
	explicit CCollision(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision(const CCollision& other);
	virtual ~CCollision() = default;

public:
	virtual HRESULT Ready_Component_Prototype() = 0; // ���� ���� �� �ʱ�ȭ
	virtual HRESULT Ready_Component(void* pArg) = 0; // ������ ���� �� �ʱ�ȭ	

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; // ������Ÿ�� ����
	virtual void Free() override;
};

END

#define __COLLISION_H__
#endif