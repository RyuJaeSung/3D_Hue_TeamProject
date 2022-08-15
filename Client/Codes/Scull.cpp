#include "stdafx.h"
#include "..\Headers\Scull.h"

#include "Tile.h"
#include "PushBox.h"
#include "FixBox.h"
#include "Player_Observer.h"
#include "Player.h"
#include "Static_Camera.h"

USING(Client)

CScull::CScull(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CScull::CScull(const CScull & other)
	: CMyObject(other)
{
}

HRESULT CScull::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CScull::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	Set_TextureColor(COLOR_NAVY);
	m_eState = STATE::STAND;
	m_fShakeTime = 0.f;

	return S_OK;
}

int CScull::Update_GameObject(float fTimeDelta)
{
	//기본값 초기화
	if (m_IsSetting)
	{
		if (nullptr == m_pTransformCom)
			return 0;

		m_pTransformCom->Get_State(&m_vStartPos, CTransform::STATE_POSITION);
		m_pTransformCom->Set_Graity(0.f);

		D3DXVECTOR3 vScale = m_pTransformCom->Get_TransformDesc().vScale;
		vScale.x *= 0.99f;

		m_pTransformCom->Set_Scale(vScale);

		m_IsSetting = false;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
	{
		switch (m_eState)
		{
		case Client::CScull::STAND:
			CollisionCheck();
			break;
		case Client::CScull::SHAKE:
			/*사운드*/
			pManagement->Play_Sound(L"a_hue scull earthquake.wav", CSound_Manager::SCULL_SHAKE);
			Shake(fTimeDelta);
			break;
		case Client::CScull::UP:
			Up(fTimeDelta);
			break;
		case Client::CScull::DOWN:
			Down(fTimeDelta);
			break;
		}

		if (FAILED(Collision_ToPlayer()))
			return E_FAIL;
	}

	return CMyObject::Update_GameObject(fTimeDelta);
}

int CScull::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CScull::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CScull::Add_Component()
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

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Scull",
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

HRESULT CScull::Collision_ToPlayer()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;

	if (SHAKE == m_eState)
		vPos = m_vStartPos;

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
			return false;

		pPlayerTransform->Go_Posion(-vInterval);
		pPlayerTransform->ClearVelocity();

		if (pPlayerTransform->Get_TransformDesc().vPosition.y < m_pTransformCom->Get_TransformDesc().vPosition.y)
		{
			CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
			if (!pPlayer)
				return E_FAIL;

			pPlayer->Set_Player_State(CPlayer::PLAYER_DEAD);
		}
	}

	return S_OK;
}

bool CScull::Collision_ToTile()
{
	if (nullptr == m_pTransformCom)
		return false;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	CTile* pTile = nullptr;
	CCollision_Cube* pTileCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Cube");

	for (int i = 0; i < iLayerSize; ++i)
	{
		pTile = (CTile*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Cube", i);
		if (nullptr == pTile)
			return false;

		pTileCollision = (CCollision_Cube*)pTile->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pTileCollision)
			return false;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pTileCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
				return true;
		else
			continue;
	}

	return false;
}

bool CScull::Collision_ToPushBox()
{
	if (nullptr == m_pTransformCom)
		return false;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	CPushBox* pPushBox = nullptr;
	CCollision_Cube* pBoxCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_PushBox");
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pPushBox = (CPushBox*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_PushBox", i);
		if (nullptr == pPushBox)
			return false;

		if (pPushBox->Get_Color() == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			continue;

		pBoxCollision = (CCollision_Cube*)pPushBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return false;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
			return true;
		else
			continue;
	}

	return false;
}

bool CScull::Collision_ToFixBox()
{
	if (nullptr == m_pTransformCom)
		return false;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfSize = tTransformDesc.vScale / 2.f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return false;

	CFixBox* pFixBox = nullptr;
	CCollision_Cube* pBoxCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_FixBox");
	if (-1 == iLayerSize)
		return false;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pFixBox = (CFixBox*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_FixBox", i);
		if (nullptr == pFixBox)
			return false;

		if (pFixBox->Get_Color() == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
			continue;

		pBoxCollision = (CCollision_Cube*)pFixBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return false;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, vPos, vHalfSize))
			return true;
		else
			continue;
	}

	return false;
}

HRESULT CScull::CollisionCheck()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
		return E_FAIL;
	D3DXVECTOR3 vPlayerPos;

	CTransform* pPlayer = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
	pPlayer->Get_State(&vPlayerPos, CTransform::STATE_POSITION);

	if ((m_vStartPos.x + 0.5f >= vPlayerPos.x && m_vStartPos.x - 0.5f <= vPlayerPos.x) &&
		(m_vStartPos.z + 0.5f >= vPlayerPos.z && m_vStartPos.z - 0.5f <= vPlayerPos.z) &&
		m_vStartPos.y - 5.f < vPlayerPos.y)
		m_eState = STATE::SHAKE;

	return S_OK;
}

HRESULT CScull::Shake(float fTimeDelta)
{
	m_fShakeTime += fTimeDelta;	
	m_pTransformCom->Set_Position(m_vStartPos);

	Shake_Effect(fTimeDelta);
	if (m_fShakeTime >= SHAKETIME)
	{
		m_eState = STATE::DOWN;
		
		D3DXVECTOR3 vPos = m_vStartPos;
		vPos.y -= 0.1f;
		m_pTransformCom->Set_Position(vPos);
		m_pTransformCom->Set_Graity(SCULLGRAVITY);
		m_fShakeTime = 0.f;
	}
	return S_OK;
}

HRESULT CScull::Up(float fTimeDelta)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
		return E_FAIL;

	D3DXVECTOR3 vMyPos;
	m_pTransformCom->Get_State(&vMyPos, CTransform::STATE_POSITION);

	if (m_vStartPos.y <= vMyPos.y)
	{
		m_pTransformCom->Set_Position(m_vStartPos);
		m_eState = STATE::STAND;
		m_pTransformCom->Set_Graity(0.f);
		m_pTransformCom->ClearVelocity();

		pManagement->StopSound(CSound_Manager::SCULL_COL);
	}

	m_pTransformCom->Free_Fall(fTimeDelta);
	return S_OK;
}

// 함수 변경됨.
HRESULT CScull::Down(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
		return E_FAIL;

	if (true == Collision_ToPushBox()
		|| true == Collision_ToFixBox()
		|| true == Collision_ToTile())
	{
		m_pTransformCom->ClearVelocity();
		m_pTransformCom->Set_Graity(-SCULLGRAVITY);
		m_eState = STATE::UP;

		int num = pManagement->Get_Current_SceneID();

		CStatic_Camera* pCamera = (CStatic_Camera*)pManagement->Get_GameObject_Pointer(num, L"Layer_Camera");
		pCamera->Set_Cam_Effect(CAM_EFFECT_SHAKING);

		/*사운드*/
		pManagement->Play_Sound(L"a_hue scull impact.wav", CSound_Manager::SCULL_COL);
		if (FAILED(Create_Dust_Effect()))
			return E_FAIL;

		return S_OK;
	}

	m_pTransformCom->Free_Fall(fTimeDelta);

	return S_OK;
}

void CScull::Shake_Effect(float _fTimeDelta)
{
	float Sign[3];
	D3DXVECTOR3 Shake_Pos;
	for (int i = 0; i < 3; ++i)
	{
		int num = rand() % 2;
		if (num == 0)
			Sign[i] = _fTimeDelta * 0.2f;
		else
			Sign[i] = -_fTimeDelta * 0.2f;
	}

	Shake_Pos.x = m_vStartPos.x + float(rand() % 10) * Sign[0];
	Shake_Pos.y = m_vStartPos.y + float(rand() % 10) * Sign[1];
	Shake_Pos.z = m_vStartPos.z + float(rand() % 10) * Sign[2];

	m_pTransformCom->Set_Position(Shake_Pos);
}

HRESULT CScull::Create_Dust_Effect()
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
		// 좀 더 높이서 떨어질때!! 추가해주세요
		/*TransformDesc.vPosition.y = -0.1f;
		int iRand = rand() % 5;
		TransformDesc.vPosition.x += (float)iRand * 0.05f;
		Ready_Layer_Effect(L"GameObject_Dust_Effect", L"Layer_Dust_Effect", TransformDesc);*/
	}
	return S_OK;
}

CScull * CScull::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScull* pInstance = new CScull(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CScull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScull::Clone_GameObject(void * pArg)
{
	CScull* pInstance = new CScull(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CScull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScull::Free()
{
	CMyObject::Free();
}
