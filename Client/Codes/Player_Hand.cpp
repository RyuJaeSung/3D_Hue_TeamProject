#include "stdafx.h"
#include "..\Headers\Player_Hand.h"
#include "Player.h"
#include "Camera_Observer.h"
#include "Player_Observer.h"
#include "PushBox.h"
#include "Ladder.h"

USING(Client)

CPlayer_Hand::CPlayer_Hand(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CPlayer_Hand::CPlayer_Hand(const CPlayer_Hand & other)
	: CGameObject(other)
{
}

void CPlayer_Hand::Move_PushBox(float _fMoved)
{
	if (nullptr == m_pPushBox)
		return;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;
	CTransform* pTransform = (CTransform*)m_pPushBox->Find_Component(__T("Com_Transform"));
	pTransform->Go_Posion(D3DXVECTOR3(_fMoved * ((CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player")))->Get_Speed(), 0.f,0.f));
}

HRESULT CPlayer_Hand::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Hand::Ready_GameObject(void * pArg)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	// 카메라 옵저버 세팅
	m_pCam_Observer = new CCamera_Observer;
	m_pPlayer_Observer = new CPlayer_Observer;
	pManagement->Subscribe(m_pCam_Observer);
	pManagement->Subscribe(m_pPlayer_Observer);
	
	m_vScale = D3DXVECTOR3(PLAYER_HAND_SCALE, PLAYER_HAND_SCALE, PLAYER_HAND_SCALE);
	m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_byTexIndex = 0;
	m_fLadderHandMove = 0.5f;
	m_ePreState = PLAYER_HAND_END;
	m_eCurState = PLAYER_HAND_END;
	//m_vPlayerScale = (*(CTransform::TRANSFORM_DESC*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player"))->Find_Component(__T("Com_Transform"))).vScale;

	return S_OK;
}

int CPlayer_Hand::Update_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	m_bLadderUpMove = false;
	m_bLadderDownMove = false;
	Change_Hand_State();
	if (m_eCurState != PLAYER_HAND_BOX_GRAP)
		m_pPushBox = nullptr;
	// 좌우 반전에 따라 스케일 값이 변하으로 실시간으로 받아야 함.
	CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player"));
	m_vPos = *pPlayer->Get_Pos();
	m_vPlayerScale = *pPlayer->Get_Scale();

	switch (dynamic_cast<CCamera_Observer*>(m_pCam_Observer)->Get_Cam_View())
	{
	case 0:	//Third Side View
			// 플레이어가 보는 위치에 따라 상자의 위치가 달라짐
		if (m_vPlayerScale.x < 0.f)		// 왼쪽
		{
			m_vPos.x -= abs(m_vPlayerScale.x);
			m_vScale = D3DXVECTOR3(-1.f * abs(m_vScale.x), m_vScale.y, m_vScale.z);
		}
		else								// 오른쪽
		{
			m_vPos.x += m_vPlayerScale.x;
			m_vScale = D3DXVECTOR3(1.f * abs(m_vScale.x), m_vScale.y, m_vScale.z);
		}
		break;
	case 1: //First Person View
		break;
	}

	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	// 충돌 체크
	if (!Collision_ToPushBox() && !Collision_ToLadder())
		m_eCurState = PLAYER_HAND_END;

	return 0;
}

int CPlayer_Hand::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	m_matWorld = matScale * matTrans;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CPlayer_Hand::Render_GameObject()
{
	if (m_eCurState != PLAYER_HAND_END)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		if (m_eCurState != PLAYER_HAND_LADDER_GRAP)
		{
			if (FAILED(m_pTextureCom->Set_Texture(int(m_byTexIndex))))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom->Set_Texture(int(m_byTexIndex))))
				return E_FAIL;

			m_matWorld._42 += m_fLadderHandMove;;
			m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
				return E_FAIL;

			m_matWorld._42 -= m_fLadderHandMove;
			m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
				return E_FAIL;

			if (m_bLadderUpMove)
				m_fLadderHandMove -= 0.05f;
			else if(m_bLadderDownMove)
				m_fLadderHandMove += 0.05f;

			if (m_fLadderHandMove <= 0.f)
				m_fLadderHandMove += 0.5f;
			else if(m_fLadderHandMove > 0.5f)
				m_fLadderHandMove -= 0.5f;
		}
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	return S_OK;
}

void CPlayer_Hand::Change_Hand_State()
{
	if (m_ePreState == m_eCurState)
		return;
	switch (m_eCurState)
	{
	case PLAYER_HAND_IDLE:
	case PLAYER_HAND_LADDER_IDLE:
	case PLAYER_HAND_BOX_IDLE:
		m_pPushBox = nullptr;
		m_byTexIndex = 0;
		break;
	case PLAYER_HAND_LADDER_GRAP:
	case PLAYER_HAND_BOX_GRAP:
		m_byTexIndex = 1;
		break;
	}
	m_ePreState = m_eCurState;
}

HRESULT CPlayer_Hand::Add_Component()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Hand",
		L"Com_Texture_Hand", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion

#pragma region Com_CubeSphere_Collision
	// For.Com_CubeSphere_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_CubeSphere",
		L"Com_Collision_CubeSphere", (CComponent**)&m_pCubeSphereCollisionCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CPlayer_Hand::SetUp_CollisionDesc()
{
	if (nullptr == m_pCubeSphereCollisionCom)
		return E_FAIL;

	D3DXVECTOR3 vHalfScale = m_vScale * 0.5f;
	if (0 > vHalfScale.x)
		vHalfScale.x = -vHalfScale.x;

	m_pCubeSphereCollisionCom->SetUp_SphereDesc(m_vPos, vHalfScale.x);

	return S_OK;
}

BOOL CPlayer_Hand::Collision_ToPushBox()
{
	D3DXVECTOR3 vHalfSize = D3DXVECTOR3(abs(m_vScale.x), m_vScale.y, m_vScale.z) * 0.5f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// 상대 PushBox
	CPushBox* pPushBox = nullptr;
	CCollision_Cube* pBoxCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_PushBox");
	DWORD dwColor = dynamic_cast<CPlayer_Observer*>(m_pPlayer_Observer)->Get_Color();
	for (int i = 0; i < iLayerSize; ++i)
	{
		pPushBox = (CPushBox*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_PushBox", i);
		if (nullptr == pPushBox)
			return E_FAIL;
	
		if(dwColor == pPushBox->Get_Color())
			continue;

		pBoxCollision = (CCollision_Cube*)pPushBox->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return E_FAIL;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, m_vPos, vHalfSize))
		{
			CTransform* pPushBoxTransform = (CTransform*)pPushBox->Find_Component(L"Com_Transform");
			if (nullptr == pPushBoxTransform)
				return E_FAIL;

			if (m_eCurState == PLAYER_HAND_BOX_GRAP)
				m_pPushBox = pPushBox;
			else if (m_eCurState != PLAYER_HAND_BOX_GRAP || m_eCurState != PLAYER_HAND_LADDER_GRAP)
				m_eCurState = PLAYER_HAND_BOX_IDLE;
			return true;
		}
		else
			continue;
	}

	return false;
}

BOOL CPlayer_Hand::Collision_ToLadder()
{
	D3DXVECTOR3 vHalfSize = D3DXVECTOR3(abs(m_vScale.x), m_vScale.y, m_vScale.z) * 0.5f;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// 상대 pLadder
	CLadder* pLadder = nullptr;
	CCollision_Cube* pBoxCollision = nullptr;
	int iLayerSize = pManagement->Get_Layer_Size(pManagement->Get_Current_SceneID(), L"Layer_Ladder");
	DWORD dwColor = dynamic_cast<CPlayer_Observer*>(m_pPlayer_Observer)->Get_Color();
	for (int i = 0; i < iLayerSize; ++i)
	{
		pLadder = (CLadder*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Ladder", i);
		if (nullptr == pLadder)
			return E_FAIL;
		if (dwColor == pLadder->Get_Color())
			continue;

		pBoxCollision = (CCollision_Cube*)pLadder->Find_Component(L"Com_Collision_Cube");
		if (nullptr == pBoxCollision)
			return E_FAIL;

		D3DXVECTOR3 vInterval = { 0.f, 0.f, 0.f };

		if (true == pBoxCollision->IsCollision_ToDestCube(&vInterval, m_vPos, vHalfSize))
		{
			if (m_ePreState == PLAYER_HAND_LADDER_GRAP)
				m_eCurState = PLAYER_HAND_LADDER_GRAP;
			else
				m_eCurState = PLAYER_HAND_LADDER_IDLE;

			return true;
		}
		else
			continue;
	}

	return false;
}

CPlayer_Hand * CPlayer_Hand::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer_Hand* pInstance = new CPlayer_Hand(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CPlayer_hand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer_Hand::Clone_GameObject(void * pArg)
{
	CPlayer_Hand* pInstance = new CPlayer_Hand(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CPlayer_hand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Hand::Free()
{
	//
	if (nullptr != m_pPushBox)
		m_pPushBox = nullptr;
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCubeSphereCollisionCom);

	Safe_Release(m_pCam_Observer);
	Safe_Release(m_pPlayer_Observer);
	//
	CGameObject::Free();
}
