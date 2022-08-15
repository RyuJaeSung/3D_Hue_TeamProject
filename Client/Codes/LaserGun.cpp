#include "stdafx.h"
#include "..\Headers\LaserGun.h"

#include "Laser.h"

USING(Client)

CLaserGun::CLaserGun(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CLaserGun::CLaserGun(const CLaserGun& other)
	: CMyObject(other)
{
}

HRESULT CLaserGun::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLaserGun::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CLaserGun::Update_GameObject(float fTimeDelta)
{
	if (false == m_bIsinit)
	{
		// Ready�� ������ ���⼳�� �ɰŶ� ����� ������Ʈ ���̿��� ������ ����
		if (FAILED(Create_Laser()))
			return 0;

		m_bIsinit = true;
	}

	if (FAILED(SetUp_Billboard()))
		return 0;

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CLaserGun::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CLaserGun::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE); // Zread�� ���� �ʰڴ�. ������ �����ִ� Z���� ������ �۰� ũ�� ��� ���� ������ �׸��ڴ�.
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Zwrite�� ���� �ʰڴ�. ���� Z���� ������� �����Ƿ� ���Ŀ� �׷��� ����� �� Z���� ���� �� ����(���� ���� �� ���̴�).

	if (FAILED(CMyObject::Render_GameObject()))
		return E_FAIL;

	//// ��ġ�� ����Ʈ ��.
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CLaserGun::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_LaserGun",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Frustum
	//For.Com_Frustum
	// �ø� ������Ʈ �߰�
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Frustum",
		L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CLaserGun::Create_Laser()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// ���� �� �޾ƿ�
	SCENE_ID eCurScene = (SCENE_ID)pManagement->Get_Current_SceneID();

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_Laser",
		eCurScene, L"Layer_Laser")))
		return E_FAIL;

	// ������ ��������� ����
	CLaser* pBuffLaser = nullptr;
	int iCount = pManagement->Get_Layer_Size(eCurScene, L"Layer_Laser"); // �ش� ���̾��� �ټ� �޾ƿ�
	if (-1 == iCount)
		return S_OK;

	while (true)
	{
		pBuffLaser = (CLaser*)pManagement->Get_GameObject_Pointer(eCurScene, L"Layer_Laser", iCount);
		if (nullptr == pBuffLaser)
			break;

		++iCount;
	}

	pBuffLaser = (CLaser*)pManagement->Get_GameObject_Pointer(eCurScene, L"Layer_Laser", iCount - 1);

	if (FAILED(pBuffLaser->Set_LaserGun(this)))
		return E_FAIL;

	// ������ ��ġ, ���� ����
	CTransform* pLaserTransform = (CTransform*)pBuffLaser->Find_Component(L"Com_Transform");
	if (nullptr == pLaserTransform)
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vScale = { 0.f, 0.1f, 0.1f }; // �������� ���� ������
	TransformInfo.fSpeedPerSec = 3.f;

	pBuffLaser->Set_RevRadian(m_pTransformCom->Get_TransformDesc().vRotate); // �������� ����

	pLaserTransform->Set_TransformDesc(TransformInfo);

	// ������ ���� ����
	pBuffLaser->Set_TextureColor(m_eColorType);
	return S_OK;
}

CLaserGun * CLaserGun::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLaserGun* pInstance = new CLaserGun(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CLaserGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaserGun::Clone_GameObject(void * pArg)
{
	CLaserGun* pInstance = new CLaserGun(*this); // ���� ����

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CLaserGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLaserGun::Free()
{
	CMyObject::Free();
}
