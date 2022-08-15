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
	virtual HRESULT Ready_Component_Prototype() = 0; // 원본 생성 시 초기화
	virtual HRESULT Ready_Component(void* pArg) = 0; // 복제본 생성 시 초기화	

public:
	// Clone
	virtual CComponent* Clone_Component(void* pArg) = 0; // 프로토타입 패턴
	virtual void Free() override;
};

END

#define __COLLISION_H__
#endif