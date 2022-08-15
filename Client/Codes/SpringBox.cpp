#include "stdafx.h"
#include "..\Headers\SpringBox.h"

#include "Player.h"

USING(Client)

CSpringBox::CSpringBox(LPDIRECT3DDEVICE9 pDevice)
	: CMyObject(pDevice)
{
}

CSpringBox::CSpringBox(const CSpringBox & other)
	: CMyObject(other)
{
}

HRESULT CSpringBox::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSpringBox::Ready_GameObject(void * pArg)
{
	if (FAILED(CMyObject::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

int CSpringBox::Update_GameObject(float fTimeDelta)
{
	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	return 	CMyObject::Update_GameObject(fTimeDelta);
}

int CSpringBox::LateUpdate_GameObject(float fTimeDelta)
{
	return CMyObject::LateUpdate_GameObject(fTimeDelta);
}

HRESULT CSpringBox::Render_GameObject()
{
	return CMyObject::Render_GameObject();
}

HRESULT CSpringBox::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_SpringBox",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Collision_Cube
	//For.Com_Collision_Cube
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_Cube",
		L"Com_Collision_Cube", (CComponent**)&m_pCubeCollisionCom)))
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

HRESULT CSpringBox::SetUp_CollisionDesc()
{
	if (nullptr == m_pCubeCollisionCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;
	if (0 > vHalfScale.x)
		vHalfScale.x = -vHalfScale.x;

	m_pCubeCollisionCom->SetUp_CubeDesc(vPos, vHalfScale);

	return S_OK;
}

HRESULT CSpringBox::Collision(float _fTimeDelta)
{
	return Collision_ToPlayer(_fTimeDelta);
}

HRESULT CSpringBox::Collision_ToPlayer(float _fTimeDelta)
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
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player");
		if (nullptr == pPlayer)
			return E_FAIL;

		CTransform* pPlayerTransform = (CTransform*)pPlayer->Find_Component(L"Com_Transform");
		if (nullptr == pPlayerTransform)
			return E_FAIL;
/*
		pPlayerTransform->Go_Posion(-vInterval);
		pPlayerTransform->ClearVelocity();*/
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
			tPlayer_TransformDesc.vVelocity.y = -0.1f;
			pPlayerTransform->Set_TransformDesc(tPlayer_TransformDesc);
		}
		else if (vInterval.y < 0.f)
			pPlayerTransform->ClearVelocity();


		if (vInterval.x == 0.f && vInterval.y < 0.f && vInterval.z == 0.f)
		{
			/*사운드*/
			pManagement->StopSound(CSound_Manager::SPRING_BOX);

			pPlayer->Change_Player_State(CPlayer::PLAYER_JUMP);
			pPlayerTransform->Jump(_fTimeDelta, pPlayer->Get_Jump_Speed() * 1.2f);
			//pPlayerTransform->Go_Up(_fTimeDelta * 2);

			int iColor = (int)m_eColorType;
			++iColor;

			if (iColor > (int)COLOR_TYPE::COLOR_PINK)
				iColor = (int)COLOR_TYPE::COLOR_RED;

			Set_TextureColor((COLOR_TYPE)iColor);

			/*사운드*/
			pManagement->Play_Sound(L"a_hue spring jump.ogg", CSound_Manager::SPRING_BOX);
		}

		if (vInterval.y != 0.f)
		{
			m_bIsTransScale = true;
			m_fLimitTime = vInterval.y;
		}
	}

	if (m_bIsTransScale)
	{
		m_vScale.y -= _fTimeDelta * 20.f;
		m_fScaleTime += _fTimeDelta;
	}
	else
	{
		if (m_vScale.y < 1.f)
			m_vScale.y += _fTimeDelta;
		else
			m_vScale.y = 1.f;
	}

	if (_fTimeDelta * m_fLimitTime * 0.5f < m_fScaleTime)
	{
		m_bIsTransScale = false;
		m_fScaleTime = 0.f;
	}

	m_pTransformCom->Set_Scale(m_vScale);
	return S_OK;
}

CSpringBox * CSpringBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpringBox* pInstance = new CSpringBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CSpringBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpringBox::Clone_GameObject(void * pArg)
{
	CSpringBox* pInstance = new CSpringBox(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CSpringBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpringBox::Free()
{
	Safe_Release(m_pCubeCollisionCom);

	CMyObject::Free();
}
