#include "stdafx.h"
#include "..\Headers\MoveBox.h"
#include "Tile.h"


USING(Client)

CMoveBox::CMoveBox(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CMoveBox::CMoveBox(const CMoveBox & other)
	: CMyObject(other)
{
}

HRESULT CMoveBox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMoveBox::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;
	return S_OK;
}

int CMoveBox::Update_GameObject(float fTimeDelta)
{
	if (m_IsSetting)
	{
		m_pTransformCom->Set_Speed(2.f);
		m_IsSetting = false;
	}

	switch (m_eDirOption)
	{
	case Client::CMoveBox::DIR_UP:
		m_vSpeed = D3DXVECTOR3(0.f, m_pTransformCom->Get_TransformDesc().fSpeedPerSec, 0.f);
		Up(fTimeDelta);
		break;
	case Client::CMoveBox::DIR_DOWN:
		m_vSpeed = D3DXVECTOR3(0.f, -m_pTransformCom->Get_TransformDesc().fSpeedPerSec, 0.f);
		Down(fTimeDelta);
		break;
	case Client::CMoveBox::DIR_LEFT:
		m_vSpeed = D3DXVECTOR3(-m_pTransformCom->Get_TransformDesc().fSpeedPerSec, 0.f, 0.f);
		Left(fTimeDelta);
		break;
	case Client::CMoveBox::DIR_RIGHT:
		m_vSpeed = D3DXVECTOR3(m_pTransformCom->Get_TransformDesc().fSpeedPerSec, 0.f, 0.f);
		Right(fTimeDelta);
		break;
	case Client::CMoveBox::DIR_NONE:
		m_vSpeed = D3DXVECTOR3(0.f, 0.f, 0.f);
		break;
	}

	if (FAILED(Collision_ToTile()))
		return 0;

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CMoveBox::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CMoveBox::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CMoveBox::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_MoveBox",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Frustum
	//For.Com_Frustum
	// 컬링 컴포넌트 추가
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Frustum",
		L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMoveBox::Collision(float _fTimeDelta)
{
	return Collision_ToPlayer(_fTimeDelta);
}

HRESULT CMoveBox::Collision_ToPlayer(float _fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCollision_CubeSphere* pCubeSphereCollision
		= (CCollision_CubeSphere*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Collision_CubeSphere");
	if (nullptr == pCubeSphereCollision)
		return E_FAIL;

	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
	{
		CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
		if (nullptr == pPlayerTransform)
			return E_FAIL;

		if (pPlayerTransform->Get_TransformDesc().vPosition.y > m_pTransformCom->Get_TransformDesc().vPosition.y)
		{
			//플레이어가 상자 위에 있을 때 충돌 후 공중에 체류하는 것을 방지하기 위한 코드
			vInterval.y *= 0.99f;
			pPlayerTransform->Go_Posion(-vInterval);
		}
		else
			pPlayerTransform->Go_Posion(-vInterval);

		if (vInterval.y > 0.f)
		{
			// 플레이어 머리와 충돌할 경우 속도가 0.f이 되어 플레이어의 상태가 PLAYER_IDLE 상태로 돌아가는 것을 방지함.
			CTransform::TRANSFORM_DESC tPlayer_TransformDesc = pPlayerTransform->Get_TransformDesc();
			tPlayer_TransformDesc.vVelocity.y = -tPlayer_TransformDesc.vVelocity.y;
			pPlayerTransform->Set_TransformDesc(tPlayer_TransformDesc);
		}
		else if (vInterval.y < 0.f)
			pPlayerTransform->ClearVelocity();
	
		pPlayerTransform->Go_Posion(m_vSpeed * _fTimeDelta);


		/*if (vInterval.y != 0.f)
			pPlayerTransform->Go_Posion(m_vSpeed * _fTimeDelta);*/
		/*사운드*/
		if (pPlayerTransform->Get_TransformDesc().vPosition.y > vPos.y)
			pManagement->StopSound(CSound_Manager::PLAYER_JUMP);
	}

	return S_OK;
}

HRESULT CMoveBox::Collision_ToTile()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTile* pTile = nullptr;
	CCollision_Cube* pTileCollision = nullptr;
	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Cube");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pTile = (CTile*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Cube", i);
		if (nullptr == pTile)
			return E_FAIL;

		CTransform* pTile_Transform = (CTransform*)pTile->Find_Component(L"Com_Transform");
		if (nullptr == pTile_Transform)
			return E_FAIL;

		CTransform::TRANSFORM_DESC tTile_TransformDesc = pTile_Transform->Get_TransformDesc();
		D3DXVECTOR3 vTilePos = tTile_TransformDesc.vPosition;
		float fLength = D3DXVec3Length(&(vTilePos - vPos));

		if (3.f > fLength)
		{
			// 충돌 검사
			pTileCollision = (CCollision_Cube*)pTile->Find_Component(L"Com_Collision_Cube");
			if (nullptr == pTileCollision)
				return E_FAIL;

			if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
			{
				if (vInterval.x != 0.f)
				{ 
					switch (m_eDirOption)
					{
					case Client::CMoveBox::DIR_RIGHT:
						m_eDirOption = DIR_LEFT;
						break;
					case Client::CMoveBox::DIR_LEFT:
						m_eDirOption = DIR_RIGHT;
						break;
					default:
						break;
					}
				}
			}
		}
		else
			continue;
	}

	return S_OK;
}


void CMoveBox::Up(float fTimeDelta)
{
	m_pTransformCom->Go_Up(fTimeDelta);

	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);

	if (m_vStartPos.y + m_fDis <= vPos.y)
		m_eDirOption = DIR_DOWN;
}

void CMoveBox::Down(float fTimeDelta)
{
	m_pTransformCom->Go_Up(-fTimeDelta);

	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);

	if (m_vStartPos.y - m_fDis >= vPos.y)
		m_eDirOption = DIR_UP;
}

void CMoveBox::Right(float fTimeDelta)
{
	m_pTransformCom->Go_Side(fTimeDelta);

	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);

	if (m_vStartPos.x + m_fDis <= vPos.x)
		m_eDirOption = DIR_LEFT;
}

void CMoveBox::Left(float fTimeDelta)
{
	m_pTransformCom->Go_Side(-fTimeDelta);

	D3DXVECTOR3 vPos;
	m_pTransformCom->Get_State(&vPos, CTransform::STATE_POSITION);

	if (m_vStartPos.x - m_fDis >= vPos.x)
		m_eDirOption = DIR_RIGHT;
}
CMoveBox * CMoveBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMoveBox* pInstance = new CMoveBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CMoveBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMoveBox::Clone_GameObject(void * pArg)
{
	CMoveBox* pInstance = new CMoveBox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CMoveBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoveBox::Free()
{
	CMyObject::Free();
}
