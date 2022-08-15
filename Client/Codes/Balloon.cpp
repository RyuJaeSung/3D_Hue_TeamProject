#include "stdafx.h"
#include "..\Headers\Balloon.h"

#include "BalloonBox.h"
#include "Tile.h"
#include "SpringBox.h"

#include "Player_Observer.h"

USING(Client)

CBalloon::CBalloon(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CBalloon::CBalloon(const CBalloon & other)
	: CMyObject(other)
{
}

void CBalloon::Set_Pos(D3DXVECTOR3 vPos)
{
	m_pTransformCom->Go_Posion(vPos);
}

HRESULT CBalloon::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBalloon::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CBalloon::Update_GameObject(float fTimeDelta)
{
	if (true == m_IsSetting)
	{
		// Ready가 끝나고 색상이랑 위치랑 다 설정 될거라서 레디와 업데이트 사이에서 박스 생성
		if (FAILED(Create_BalloonBox()))
			return 0;

		m_IsSetting = false;
	}

	if (FAILED(Curling()))
		return 0;

	if (FAILED(Collision_ToSpringBox(fTimeDelta)))
		return 0;

	if (false == m_bCollision)
	{
		if (FAILED(Collision_ToTile(fTimeDelta)))
			return 0;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CBalloon::LateUpdate_GameObject(float fTimeDelta)
{
	m_bCollision = false;

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CBalloon::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CBalloon::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.fSpeedPerSec = 1.f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Balloon",
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

HRESULT CBalloon::Create_BalloonBox()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SCENE_ID eCurScene = (SCENE_ID)pManagement->Get_Current_SceneID();

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_BalloonBox",
		eCurScene, L"Layer_BalloonBox")))
		return E_FAIL;

	// 레이저 멤버변수로 참조
	int iCount = pManagement->Get_Layer_Size(eCurScene, L"Layer_BalloonBox"); // 해당 레이어의 겟수 받아옴
	if (-1 == iCount)
		return S_OK;

	m_pBalloonBox = (CBalloonBox*)pManagement->Get_GameObject_Pointer(eCurScene, L"Layer_BalloonBox", iCount - 1);
	if (nullptr == m_pBalloonBox)
		return 0;

	//Safe_AddRef(m_pBalloonBox);

	if (FAILED(m_pBalloonBox->Set_Balloon(this)))
		return E_FAIL;

	D3DXVECTOR3 vBalloonPosition = m_pTransformCom->Get_TransformDesc().vPosition;
	D3DXVECTOR3 vBalloonScale = m_pTransformCom->Get_TransformDesc().vScale;

	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = vBalloonPosition;
	TransformInfo.vPosition.y = (vBalloonPosition.y - (vBalloonScale.y / 2.f) - 0.5f) / 2.f; // 풍선 아래로 위치 잡아줌

	TransformInfo.fSpeedPerSec = 1.f;

	// 레이저 위치, 방향 설정
	CTransform* pLaserTransform = (CTransform*)m_pBalloonBox->Find_Component(L"Com_Transform");
	if (nullptr == pLaserTransform)
		return E_FAIL;

	pLaserTransform->Set_TransformDesc(TransformInfo);

	// 레이저 색상 설정
	m_pBalloonBox->Set_TextureColor(m_eBoxColor);

	return S_OK;
}

HRESULT CBalloon::Collision_ToTile(float _fTimeDelta)
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

		// 충돌 검사
		pTileCollision = (CCollision_Cube*)pTile->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pTileCollision)
			return E_FAIL;

		if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
		{
			if (vInterval.y != 0.f && m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			{
				m_bCollision = true;
				return S_OK;
			}

			if (m_dwColor == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
				break;

			continue;
		}
		else
			continue;
	}

	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
	{
		// 풍선 위로 올리기
		m_pTransformCom->Set_Speed(1.f);
		m_pTransformCom->Go_Up(_fTimeDelta);
	}
	else
	{
		// 풍선 아래로 내리기
		if (m_bMove)
		{
			m_pTransformCom->Set_Speed(-1.f);
			m_pTransformCom->Go_Up(_fTimeDelta);
		}
	}

	return S_OK;
}

HRESULT CBalloon::Collision_ToSpringBox(float _fTimeDelta)
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

		// 충돌 검사
		pTileCollision = (CCollision_Cube*)pBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pTileCollision)
			return E_FAIL;

		if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
		{
			if (vInterval.y != 0.f && m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			{
				m_bCollision = true;
				return S_OK;
			}

			if (m_dwColor == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
				break;

			continue;
		}
		else
			continue;
	}

	return S_OK;
}

CBalloon * CBalloon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBalloon* pInstance = new CBalloon(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CBalloon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBalloon::Clone_GameObject(void * pArg)
{
	CBalloon* pInstance = new CBalloon(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CFixBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalloon::Free()
{
	CMyObject::Free();
}