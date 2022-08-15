#include "..\Headers\Collision.h"
#include "GameObject.h"

USING(Engine)

CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CCollision::CCollision(const CCollision & other)
	: CComponent(other)
{
	
}

HRESULT CCollision::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollision::Ready_Component(void * pArg)
{
	return S_OK;
}

void CCollision::Free()
{
	CComponent::Free();
}
