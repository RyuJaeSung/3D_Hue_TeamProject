#include "stdafx.h"
#include "..\Headers\BalloonBox.h"

#include "Balloon.h"
#include "Tile.h"
#include "SpringBox.h"
#include"Player_Observer.h"

USING(Client)

CBalloonBox::CBalloonBox(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CBalloonBox::CBalloonBox(const CBalloonBox & other)
	: CMyObject(other)
{
}

HRESULT CBalloonBox::Set_Balloon(CBalloon * _pIn)
{
	if (nullptr == _pIn)
		return E_FAIL;

	m_pBalloon = _pIn;
	Safe_AddRef(m_pBalloon);

	return S_OK;
}

HRESULT CBalloonBox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBalloonBox::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CBalloonBox::Update_GameObject(float fTimeDelta)
{
	// 풍선의 위치 값에 따라서 움직인다.
	if (FAILED(SetUp_BoxPosition()))
		return 0;

	if (FAILED(Curling()))
		return 0;

	if (FAILED(Collision_ToSpringBox()))
		return 0;

	if (false == m_bCollision)
	{
		if (FAILED(Collision_ToTile()))
			return 0;
	}

	if (FAILED(Collision_ToPlayer(fTimeDelta)))
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CBalloonBox::LateUpdate_GameObject(float fTimeDelta)
{
	m_bCollision = false;

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CBalloonBox::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CBalloonBox::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	// 임의로 하나 생성
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = { 1.f, 3.f, 0.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_BalloonBox",
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

HRESULT CBalloonBox::SetUp_BoxPosition()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform* pBalloon_Transform = (CTransform*)m_pBalloon->Find_Component(L"Com_Transform");
	if (nullptr == pBalloon_Transform)
		return E_FAIL;

	// 풍선 Transform
	CTransform::TRANSFORM_DESC tBalloon_Desc = pBalloon_Transform->Get_TransformDesc();
	D3DXVECTOR3 vBalloonPosition = tBalloon_Desc.vPosition;
	D3DXVECTOR3 vBalloonScale = tBalloon_Desc.vScale;

	// 풍선 상자 Transform
	D3DXVECTOR3 vBoxPos = m_pTransformCom->Get_TransformDesc().vPosition;
	D3DXVECTOR3 vBoxScale = m_pTransformCom->Get_TransformDesc().vScale;

	vBoxPos.y = vBalloonPosition.y - (vBalloonScale.y / 2.f) - (vBoxScale.y / 2.f);

	m_pTransformCom->Set_Position(vBoxPos);

	return S_OK;
}

HRESULT CBalloonBox::Collision_ToPlayer(float _fTimeDelta)
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

		pPlayerTransform->ClearVelocity();

		CTransform* pBalloon_Transform = (CTransform*)m_pBalloon->Find_Component(L"Com_Transform");
		if (nullptr == pBalloon_Transform)
			return E_FAIL;

		float vBalloon_Speed = pBalloon_Transform->Get_TransformDesc().fSpeedPerSec;

		pPlayerTransform->Go_Posion(D3DXVECTOR3(0.f, vBalloon_Speed, 0.f) * _fTimeDelta);

	}

	return S_OK;
}

HRESULT CBalloonBox::Collision_ToTile()
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

		CTransform::TRANSFORM_DESC tTile_TransformDesc = ((CTransform*)pTile->Find_Component(L"Com_Transform"))->Get_TransformDesc();
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
				if (vInterval.y != 0.f)
				{
					m_pBalloon->Set_Move(false);
					return S_OK;
				}

				continue;
			}
			else
				continue;
		}
		else
			continue;
	}

	m_pBalloon->Set_Move(true);

	return S_OK;
}

HRESULT CBalloonBox::Collision_ToSpringBox()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CSpringBox* pBox = nullptr;
	CCollision_Cube* pTileCollision = nullptr;
	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_SpringBox");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pBox = (CSpringBox*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_SpringBox", i);
		if (nullptr == pBox)
			return E_FAIL;

		if (pBox->Get_Color() == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			continue;

		CTransform::TRANSFORM_DESC tTile_TransformDesc = ((CTransform*)pBox->Find_Component(L"Com_Transform"))->Get_TransformDesc();
		D3DXVECTOR3 vTilePos = tTile_TransformDesc.vPosition;
		float fLength = D3DXVec3Length(&(vTilePos - vPos));

		if (3.f > fLength)
		{
			// 충돌 검사
			pTileCollision = (CCollision_Cube*)pBox->Find_Component(L"Com_Collision_Cube");
			if (nullptr == pTileCollision)
				return E_FAIL;

			if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
			{
				if (vInterval.y != 0.f)
				{
					m_bCollision = true;
					m_pBalloon->Set_Move(false);
					return S_OK;
				}

				continue;
			}
			else
				continue;
		}
		else
			continue;
	}
	return S_OK;
}

CBalloonBox * CBalloonBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBalloonBox* pInstance = new CBalloonBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CBalloonBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBalloonBox::Clone_GameObject(void * pArg)
{
	CBalloonBox* pInstance = new CBalloonBox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CBalloonBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalloonBox::Free()
{
	Safe_Release(m_pBalloon);

	CMyObject::Free();
}