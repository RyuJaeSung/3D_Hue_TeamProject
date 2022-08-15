#include "..\Headers\GameObject.h"
#include "Management.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& other)
	: m_pDevice(other.m_pDevice)
{
	Safe_AddRef(m_pDevice);
}

CComponent* CGameObject::Find_Component(const wstring & ComponentTag)
{
	auto iter_find = m_Components.find(ComponentTag);
	if (m_Components.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

bool CGameObject::Is_Alived() const
{
	return m_IsAlived;
}

void CGameObject::Dead_Ailved()
{
	m_IsAlived = false;
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Ready_GameObject(void * pArg)
{
	return S_OK;
}

int CGameObject::Update_GameObject(float fTimeDelta)
{
	return 0;
}

int CGameObject::LateUpdate_GameObject(float fTimeDelta)
{
	return 0;
}

HRESULT CGameObject::Render_GameObject()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(int iSceneID, const wstring& PrototypeTag, const wstring& ComponentTag, CComponent** ppComponent, void * pArg)
{
	if (nullptr != Find_Component(ComponentTag))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CComponent* pComponent = pManagement->Clone_Component(iSceneID, PrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.insert(make_pair(ComponentTag, pComponent));

	if (nullptr != ppComponent)
	{
		*ppComponent = pComponent;
		Safe_AddRef(pComponent);
	}

	return S_OK;
}

void CGameObject::Free()
{
	for (auto& MyPair : m_Components)
		Safe_Release(MyPair.second);
	
	m_Components.clear();

	Safe_Release(m_pDevice);
}
