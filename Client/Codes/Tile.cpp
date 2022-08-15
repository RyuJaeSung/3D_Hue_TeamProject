#include "stdafx.h"
#include "..\Headers\Tile.h"

#include "Camera.h"

USING(Client)

CTile::CTile(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CTile::CTile(const CTile & other)
	: CGameObject(other)
{
}

HRESULT CTile::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTile::Ready_GameObject(void * pArg)
{
	return S_OK;
}

int CTile::Update_GameObject(float fTimeDelta)
{
	if (m_bIsinit)
	{
		// �̵����� �����Ƿ� ó���� �ѹ��� ��ġ ����
		if (FAILED(SetUp_CollisionDest()))
			return 0;

		m_bIsinit = false;
	}

	if (FAILED(Curling()))
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	if (m_bInFrustum)
	{
		if (FAILED(Collision_ToPlayer()))
			return 0;
	}

	return 0;
}

int CTile::LateUpdate_GameObject(float fTimeDelta)
{
	if (m_bInFrustum)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
			return 0;
	}

	return 0;
}

HRESULT CTile::Render_GameObject()
{
	return S_OK;
}

HRESULT CTile::Add_Component()
{
	return S_OK;
}

HRESULT CTile::Collision_ToPlayer()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	// Ÿ�� ��ġ ���ϱ�
	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// �÷��̾� ��ġ ���ϱ�
	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tPlayer_TransformDesc = pPlayerTransform->Get_TransformDesc();
	D3DXVECTOR3 vPlayerPos = tPlayer_TransformDesc.vPosition;
	if (3.f < D3DXVec3Length(&(vPos - vPlayerPos)))
		return S_OK;

	CCollision_CubeSphere* pCubeSphereCollision
		= (CCollision_CubeSphere*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Collision_CubeSphere");
	if (nullptr == pCubeSphereCollision)
		return E_FAIL;

	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
	{
		if (pPlayerTransform->Get_TransformDesc().vPosition.y > m_pTransformCom->Get_TransformDesc().vPosition.y)
		{
			//�÷��̾ ���� ���� ���� �� �浹 �� ���߿� ü���ϴ� ���� �����ϱ� ���� �ڵ�
			vInterval.y *= 0.99f;
			pPlayerTransform->Go_Posion(-vInterval);

			/*����*/
			if(vInterval.y < 0.f)
			pManagement->StopSound(CSound_Manager::PLAYER_JUMP);
		}
		else
			pPlayerTransform->Go_Posion(-vInterval);
		// �÷��̾ Ÿ�Ϻ��� ���� ����
		//pPlayerTransform->Go_Posion(-vInterval);
		
		if(vInterval.y > 0.f)
		{
			// �÷��̾� �Ӹ��� �浹�� ��� �ӵ��� 0.f�� �Ǿ� �÷��̾��� ���°� PLAYER_IDLE ���·� ���ư��� ���� ������.
 			CTransform::TRANSFORM_DESC tSetDesc = tPlayer_TransformDesc;
			tSetDesc.vVelocity.y = -0.1f;
			pPlayerTransform->Set_TransformDesc(tSetDesc);
		}
		else if(vInterval.y < 0.f)
			pPlayerTransform->ClearVelocity();
	}

	return S_OK;
}

HRESULT CTile::Curling()
{
	if (nullptr == m_pFrustumCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	CCamera::CAMERA_DESC tCameraDesc = pCamera->Get_CameraDesc();

	//// �������, ����� ������
	//D3DXMATRIX matProjection, matView;
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	m_pFrustumCom->ConstructFrustum(tCameraDesc.fFar, tCameraDesc.matProj, tCameraDesc.matView);

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();

	m_bInFrustum = m_pFrustumCom->Check_Cube(tTransformDesc.vPosition, tTransformDesc.vScale / 2.f);

	return S_OK;
}

HRESULT CTile::SetUp_CollisionDest()
{
	if (nullptr == m_pCubeCollisionCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();

	D3DXVECTOR3 vCenterPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2, f;

	m_pCubeCollisionCom->SetUp_CubeDesc(vCenterPos, vHalfScale);

	return S_OK;
}

CGameObject * CTile::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CTile::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pCubeCollisionCom);

	CGameObject::Free();
}
