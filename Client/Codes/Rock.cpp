#include "stdafx.h"
#include "..\Headers\Rock.h"

#include "Tile.h"
#include "Player.h"

USING(Client)

CRock::CRock(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CRock::CRock(const CRock & other)
	: CMyObject(other)
{
}

void CRock::Set_StartPos(const D3DXVECTOR3 & _vPos)
{
	m_tStartInfo.vStartPos = _vPos;
}

void CRock::Set_StartColor(const COLOR_TYPE & _eColor)
{
	m_tStartInfo.eColor = _eColor;
}

HRESULT CRock::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRock::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	// 테스트용 > 나중엔 툴에서 불러오기하면 생성 후 색상 설정
	if (pArg)
	{
		BaseInfo tInfo = *((BaseInfo*)pArg);
		m_tStartInfo.vStartPos = tInfo.vStartPos;
		m_tStartInfo.eColor = tInfo.eColor;
		Set_TextureColor(tInfo.eColor);
	}
	return S_OK;
}

int CRock::Update_GameObject(float fTimeDelta)
{
	//기본값 초기화
	if (m_IsSetting)
	{
		m_pTransformCom->Set_Position(m_tStartInfo.vStartPos);
		m_pTransformCom->Set_Scale(D3DXVECTOR3(0.9f, 0.9f, 0.1f));
		m_pTransformCom->Set_Graity(((rand() % 30 + 50) * 0.05f));
		m_pTransformCom->Set_Speed(float(rand() % 5 + 1));
		m_IsSetting = false;
	}
	
	if (m_IsDead && m_fDeadCount >= 6.f)
	{
		return DEAD;
	}
	else if (m_IsDead)
	{
		m_fDeadCount += fTimeDelta * 5.f;
		if (m_fDeadCount >= 6.f)
			m_fDeadCount = 6.f;
		return ALIVE;
	}


	CMyObject::Update_GameObject(fTimeDelta);

	if (m_IsCreate)
	{
		m_fCreateTime += fTimeDelta;

		if (m_fCreateTime >= CREATETIME || m_IsAlived == false)
		{
			CManagement* pManagement = CManagement::Get_Instance();
			if (!pManagement)
				return -1;

			if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_Rock", pManagement->Get_Current_SceneID(), L"Layer_Rock", &m_tStartInfo)))
				return E_FAIL;

			m_fCreateTime = 0.f;
			m_IsCreate = false;
		}
	}

	MoveRock(fTimeDelta);
	m_pTransformCom->ClearVelocity();

	return 0;
}

int CRock::LateUpdate_GameObject(float fTimeDelta)
{

	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CRock::Render_GameObject()
{
	if (false == m_bRender)
	{
		m_bRender = true;
		return S_OK;
	}

	m_pTextureCom = m_pTextureRock[m_IsDead];
	if (m_IsDead)
		Set_TextureIndex((int)m_fDeadCount);

	return CMyObject::Render_GameObject();
}

HRESULT CRock::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	////For.Com_Transform
	// 임의로 하나 생성 (테스트)


	CTransform::TRANSFORM_DESC TransformInfo;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture_Rock
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Rock",
		L"Com_Texture", (CComponent**)&m_pTextureRock[ALIVE])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture_RockDead
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_RockDead",
		L"Com_Texture_Dead", (CComponent**)&m_pTextureRock[DEAD])))
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

void CRock::MoveRock(float fTimeDelta)
{
	switch (m_eRockDir)
	{
	case Client::CRock::LEFT:
		m_pTransformCom->Go_Side(fTimeDelta);
		m_pTransformCom->Free_Fall(fTimeDelta);
		break;
	case Client::CRock::RIGHT:
		m_pTransformCom->Go_Side(-fTimeDelta);
		m_pTransformCom->Free_Fall(fTimeDelta);
		break;
	default:
		m_pTransformCom->Free_Fall(fTimeDelta);
		break;
	}
}

HRESULT CRock::Collision(float fTimeDelta)
{
	if (FAILED(Collision_ToPlayer()))
		return S_OK;

	if (FAILED(Collision_ToTile()))
		return S_OK;

	return S_OK;
}

HRESULT CRock::Collision_ToPlayer()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	CCollision_CubeSphere* pCubeSphereCollision = (CCollision_CubeSphere*)pPlayer->Find_Component(L"Com_Collision_CubeSphere");
	if (nullptr == pCubeSphereCollision)
		return E_FAIL;

	D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

	if (true == pCubeSphereCollision->IsCollision_ToDestSphere(&vInterval, vPos, vHalfScale))
		pPlayer->Set_Player_State(CPlayer::PLAYER_DEAD);

	return S_OK;
}

HRESULT CRock::Collision_ToTile()
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
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Cube");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pTile = (CTile*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Cube", i);
		if (nullptr == pTile)
			return E_FAIL;

		pTileCollision = (CCollision_Cube*)pTile->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pTileCollision)
			return E_FAIL;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
		{
			Create_Dust_Effect();
			m_IsDead = true;
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT CRock::Create_Dust_Effect()
{
	D3DXVECTOR3 vPlayerPos = m_pTransformCom->Get_TransformDesc().vPosition;
	CTransform::TRANSFORM_DESC TransformDesc;

	for (float i = -2; i < 3; ++i)
	{
		D3DXVECTOR3 vPosition = vPlayerPos;
		vPosition.x += i*0.2f;
		vPosition.y -= 0.3f;
		TransformDesc.vPosition = vPosition;
		TransformDesc.vScale = { 0.3f + (abs(i) *0.1f), 0.3f + (abs(i) * 0.01f), 0.3f + (abs(i) * 0.1f) };
		TransformDesc.fSpeedPerSec = i;
		if (FAILED(Ready_Layer_Effect(L"GameObject_Dust_Effect", L"Layer_Dust_Effect", TransformDesc)))
			return E_FAIL;

		TransformDesc.vPosition.y = -0.1f;
		int iRand = rand() % 5;
		TransformDesc.vPosition.x += (float)iRand * 0.05f;
		Ready_Layer_Effect(L"GameObject_Dust_Effect", L"Layer_Dust_Effect", TransformDesc);
	}
	return S_OK;
}


CRock * CRock::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRock* pInstance = new CRock(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRock::Clone_GameObject(void * pArg)
{
	CRock* pInstance = new CRock(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock::Free()
{
	m_pTextureCom = nullptr;

	for (int i = 0; i < ROCK_END; ++i)
		Safe_Release(m_pTextureRock[i]);

	CMyObject::Free();
}
