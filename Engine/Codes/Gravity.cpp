#include "..\Headers\Gravity.h"

USING(Engine)

CGravity::CGravity(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CGravity::CGravity(const CGravity & other)
	:CComponent(other)
{
}

HRESULT CGravity::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CGravity::Ready_Component(void * pArg)
{
	return S_OK;
}

const D3DXVECTOR3 & CGravity::Get_Gravity()
{
	return m_tGravity.vGravity;
}

const D3DXVECTOR3 & CGravity::Get_Velocity()
{
	return m_tGravity.vVelocity;
}

void CGravity::Set_floatGravity(const float _fGravity)
{
	m_tGravity.vGravity = D3DXVECTOR3(0.f, -_fGravity, 0.f);
}

void CGravity::Set_Gravity(const D3DXVECTOR3 & _vGravity)
{
	m_tGravity.vGravity = _vGravity;
}

void CGravity::Free_Fall(D3DXVECTOR3 & _vPosition, float _fDeltaTime)
{
	m_tGravity.vVelocity += m_tGravity.vGravity * _fDeltaTime;
	_vPosition += m_tGravity.vVelocity;
}

void CGravity::Clear_Velocity()
{
	ZeroMemory(&m_tGravity.vVelocity, sizeof(D3DXVECTOR3));
}

void CGravity::Clear_Gravity()
{
	ZeroMemory(&m_tGravity.vGravity, sizeof(D3DXVECTOR3));
}

CGravity * CGravity::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGravity* pInstance = new CGravity(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		ERR_MSG(L"Failed To Creating Gravity Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CGravity::Clone_Component(void * pArg)
{
	CGravity* pInstance = new CGravity(*this);
	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		ERR_MSG(L"Failed To Clone Gravity");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGravity::Free()
{
	CComponent::Free();
}
