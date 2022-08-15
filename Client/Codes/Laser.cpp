#include "stdafx.h"
#include "..\Headers\Laser.h"

#include "LaserGun.h"
#include "Laser_Effect.h"
#include "Tile.h"
#include "Player_Observer.h"
#include "Player.h"

USING(Client)

CLaser::CLaser(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CLaser::CLaser(const CLaser & other)
	: CMyObject(other)
{
}

HRESULT CLaser::Set_LaserGun(CLaserGun * _pIn)
{
	if (nullptr == _pIn)
		return E_FAIL;

	m_pLaserGun = _pIn;
	Safe_AddRef(m_pLaserGun);

	return S_OK;
}

HRESULT CLaser::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLaser::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CLaser::Update_GameObject(float fTimeDelta)
{
	if (false == m_bIsInit)
	{
		if (D3DXToRadian(180.f) == m_vRevRadian.z) // 왼쪽 방향
			m_vLaserDir = { -1.f, 0.f, 0.f };

		if (D3DXToRadian(0.f) == m_vRevRadian.z) // 오른쪽 방향
			m_vLaserDir = { 1.f, 0.f, 0.f };

		if (D3DXToRadian(90.f) == m_vRevRadian.z) // 위쪽 방향
			m_vLaserDir = { 0.f, 1.f, 0.f };

		if (D3DXToRadian(270.f) == m_vRevRadian.z) // 아래쪽 방향
			m_vLaserDir = { 0.f, -1.f, 0.f };

		m_bIsInit = true;
	}

	if (FAILED(Movement(fTimeDelta)))
		return 0;

	if (FAILED(FrameMove(fTimeDelta, 2.f)))
		return 0;

	// 레이저는 공전때문에 World행렬을 직접 정해주므로 부모 Update 안탐
	if (m_bInFrustum)
	{
		m_fMaxLength = 0.f; // 0으로 초기화 안시켜 주면 이전 최단거리 갱신 안됨
		Collision(fTimeDelta);
	}

	return 0;
}

int CLaser::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CLaser::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CLaser::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	//For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Laser",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Collision_Raycast
	//For.Com_Collision_Cube
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Raycast",
		L"Com_Collision_Raycast", (CComponent**)&m_pRaycast_CollisionCom)))
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

HRESULT CLaser::Collision(float _fTimeDelta)
{
	if (FAILED(Collision_ToPlayer()))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_MoveBox")))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_FixBox")))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_Scull")))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_PushBox")))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_BalloonBox")))
		return E_FAIL;

	if (FAILED(Collision_ToObject(L"Layer_SpringBox")))
		return E_FAIL;

	if (FAILED(Collision_ToTile()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLaser::Collision_ToTile()
{
	if (nullptr == m_pTransformCom
		|| nullptr == m_pRaycast_CollisionCom)
		return E_FAIL;

	float fBuffLength = 0.f;

	CTransform* pGun_Transform = (CTransform*)m_pLaserGun->Find_Component(L"Com_Transform");
	if (nullptr == pGun_Transform)
		return E_FAIL;

	D3DXVECTOR3 vRayPos = pGun_Transform->Get_TransformDesc().vPosition; // 광선의 시작점

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTile* pTile = nullptr;
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
		D3DXVECTOR3 vTileHalfSize = tTile_TransformDesc.vScale / 2.f;

		if (D3DXToRadian(180.f) == m_vRevRadian.z) // 왼쪽 방향
		{
			if (vTilePos.x >= vRayPos.x)
				continue;
		}

		if (D3DXToRadian(0.f) == m_vRevRadian.z) // 오른쪽 방향
		{
			if (vTilePos.x <= vRayPos.x)
				continue;
		}

		if (D3DXToRadian(90.f) == m_vRevRadian.z) // 위쪽 방향
		{
			if (vTilePos.y <= vRayPos.y)
				continue;
		}

		if (D3DXToRadian(270.f) == m_vRevRadian.z) // 아래쪽 방향
		{
			if (vTilePos.y >= vRayPos.y)
				continue;
		}

		if (true == m_pRaycast_CollisionCom->IsCollsion_ToCube(&fBuffLength, vRayPos, m_vLaserDir, vTilePos, vTileHalfSize))
		{
			if (fBuffLength < m_fMaxLength
				|| 0.f >= m_fMaxLength)
			{
				m_fMaxLength = fBuffLength;

				vTileHalfSize.x *= m_vLaserDir.x;
				vTileHalfSize.y *= m_vLaserDir.y;
				vTileHalfSize.z *= m_vLaserDir.z;

				m_vLaser_EffectPos = vTilePos - vTileHalfSize;
			}
		}
	}

	return S_OK;
}

HRESULT CLaser::Collision_ToObject(const wstring& _wstrObjLayerTag)
{
	if (nullptr == m_pTransformCom
		|| nullptr == m_pRaycast_CollisionCom)
		return E_FAIL;

	float fBuffLength = 0.f;

	CTransform* pGun_Transform = (CTransform*)m_pLaserGun->Find_Component(L"Com_Transform");
	if (nullptr == pGun_Transform)
		return E_FAIL;

	D3DXVECTOR3 vRayPos = pGun_Transform->Get_TransformDesc().vPosition; // 광선의 시작점

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CMyObject* pObject = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), _wstrObjLayerTag);
	if (-1 == iLayerSize)
		return S_OK;

	for (int i = 0; i < iLayerSize; ++i)
	{
		pObject = (CMyObject*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), _wstrObjLayerTag, i);
		if (nullptr == pObject)
			return E_FAIL;

		if (dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color() == pObject->Get_Color())
			continue;

		CTransform* pObj_Transform = (CTransform*)pObject->Find_Component(L"Com_Transform");
		if (nullptr == pObj_Transform)
			return E_FAIL;

		CTransform::TRANSFORM_DESC tBox_TransformDesc = pObj_Transform->Get_TransformDesc();
		D3DXVECTOR3 vObjPos = tBox_TransformDesc.vPosition;
		D3DXVECTOR3 vObjHalfSize = tBox_TransformDesc.vScale / 2.f;

		if (D3DXToRadian(180.f) == m_vRevRadian.z) // 왼쪽 방향
		{
			if (vObjPos.x >= vRayPos.x)
				continue;
		}

		if (D3DXToRadian(0.f) == m_vRevRadian.z) // 오른쪽 방향
		{
			if (vObjPos.x <= vRayPos.x)
				continue;
		}

		if (D3DXToRadian(90.f) == m_vRevRadian.z) // 위쪽 방향
		{
			if (vObjPos.y <= vRayPos.y)
				continue;
		}

		if (D3DXToRadian(270.f) == m_vRevRadian.z) // 아래쪽 방향
		{
			if (vObjPos.y >= vRayPos.y)
				continue;
		}

		if (true == m_pRaycast_CollisionCom->IsCollsion_ToCube(&fBuffLength, vRayPos, m_vLaserDir, vObjPos, vObjHalfSize))
		{
			if (fBuffLength < m_fMaxLength
				|| 0.f >= m_fMaxLength)
			{
				m_fMaxLength = fBuffLength;
				m_vLaser_EffectPos = vRayPos + (m_vLaserDir * m_fMaxLength);
			}
		}
	}

	return S_OK;
}

HRESULT CLaser::Collision_ToPlayer()
{
	if (nullptr == m_pTransformCom
		|| nullptr == m_pRaycast_CollisionCom)
		return E_FAIL;

	if (m_dwColor == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color())
		return S_OK;

	float fBuffLength = 0.f;

	CTransform* pGun_Transform = (CTransform*)m_pLaserGun->Find_Component(L"Com_Transform");
	if (nullptr == pGun_Transform)
		return E_FAIL;

	D3DXVECTOR3 vRayPos = pGun_Transform->Get_TransformDesc().vPosition; // 광선의 시작점

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
	if (nullptr == pPlayer)
		return E_FAIL;

	D3DXVECTOR3 vPlayerPos = *pPlayer->Get_Pos();
	D3DXVECTOR3 vPlayerHalfSize = *pPlayer->Get_Scale() / 2.f;

	if (true == m_pRaycast_CollisionCom->IsCollsion_ToCube(&fBuffLength, vRayPos, m_vLaserDir, vPlayerPos, vPlayerHalfSize))
	{
		if (fBuffLength < m_fLength)
			pPlayer->Set_Player_State(CPlayer::PLAYER_DEAD);
	}

	return S_OK;
}

HRESULT CLaser::Movement(float _fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	D3DXVECTOR3 vScale = { 0.f, 0.1f, 0.1f };
	D3DXVECTOR3 vPosition = { 0.f, 0.f, 0.f };

	// 레이저 증가
	m_fLength += m_pTransformCom->Get_TransformDesc().fSpeedPerSec * _fTimeDelta;

	if (m_fMaxLength < m_fLength)
	{
		m_fLength = m_fMaxLength;

		if (FAILED(Create_Laser_Effect()))
			return E_FAIL;
	}

	vScale.x = m_fLength;
	vPosition.x = m_fLength / 2.f + 0.1f;

	D3DXMATRIX matScale, matTrans, matRevolution, matParent, matWorld;

	// 크기
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);

	// 이동
	D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, vPosition.z);

	// 공전
	D3DXMatrixRotationX(&matRevolution, m_vRevRadian.x);
	D3DXMatrixRotationY(&matRevolution, m_vRevRadian.y);
	D3DXMatrixRotationZ(&matRevolution, m_vRevRadian.z);

	CTransform* pLaserGun_Transform = (CTransform*)m_pLaserGun->Find_Component(L"Com_Transform");
	if (nullptr == pLaserGun_Transform)
		return E_FAIL;

	D3DXVECTOR3 vParent_Position = pLaserGun_Transform->Get_TransformDesc().vPosition;

	// 부모
	D3DXMatrixTranslation(&matParent, vParent_Position.x, vParent_Position.y, vParent_Position.z);

	matWorld = matScale * matTrans * matRevolution * matParent;
	m_pTransformCom->Set_StateMatrix(matWorld);

	m_pTransformCom->Set_Scale(D3DXVECTOR3(matWorld._11, matWorld._22, matWorld._33));
	m_pTransformCom->Set_Position(D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43));
	return S_OK;
}

HRESULT CLaser::FrameMove(float _fTimeDelta, float _fFrameSpeed)
{
	m_fCurTextureIndex += m_fMaxTextureIndex * _fTimeDelta * _fFrameSpeed;

	if (m_fCurTextureIndex >= m_fMaxTextureIndex)
		m_fCurTextureIndex = 0.f;

	m_iTextureIndex = (int)m_fCurTextureIndex;

	return S_OK;
}

HRESULT CLaser::Create_Laser_Effect()
{
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vPosition = m_vLaser_EffectPos;
	TransformInfo.vScale = { 0.1f, 0.1f, 0.1f };
	Ready_Layer_Effect(L"GameObject_Laser_Effect", L"Layer_Laser_Effect", TransformInfo);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Laser_Effect");
	CLaser_Effect* pLaserEffect = nullptr;
	pLaserEffect = (CLaser_Effect*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Laser_Effect", iLayerSize - 1);
	if (nullptr == pLaserEffect)
		return E_FAIL;

	int iHalfRand = rand() % 2;

	D3DXVECTOR3 vEffectDir = -(m_vLaserDir * 10.f);

	if (m_vLaserDir.x < 0)
		vEffectDir.x = (float)(rand() % 10);
	else if (m_vLaserDir.x > 0)
		vEffectDir.x = (float)(rand() % 10) * (-1.f);
	else if (0 == iHalfRand)
		vEffectDir.x = (float)(rand() % 10);
	else
		vEffectDir.x = (float)(rand() % 10) * (-1.f);

	if (m_vLaserDir.y < 0)
		vEffectDir.y = (float)(rand() % 10);
	else if (m_vLaserDir.y > 0)
		vEffectDir.y = (float)(rand() % 10) * (-1.f);
	else if (0 == iHalfRand)
		vEffectDir.y = (float)(rand() % 10);
	else
		vEffectDir.y = (float)(rand() % 10) * (-1.f);

	D3DXVec3Normalize(&vEffectDir, &vEffectDir);

	pLaserEffect->Set_Dir(vEffectDir);
	pLaserEffect->Set_TextureColor(m_eColorType);

	return S_OK;
}

HRESULT CLaser::Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// 위치만 세팅해서 넘긴다

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, GameObjectTag, pManagement->Get_Current_SceneID(), LayerTag, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CLaser * CLaser::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLaser* pInstance = new CLaser(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaser::Clone_GameObject(void * pArg)
{
	CLaser* pInstance = new CLaser(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLaser::Free()
{
	Safe_Release(m_pLaserGun);
	Safe_Release(m_pRaycast_CollisionCom);

	CMyObject::Free();
}