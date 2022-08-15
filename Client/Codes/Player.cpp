#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Camera_Observer.h"
#include "Non_Player_Character.h"
#include "Player_Hand.h"

USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CPlayer::CPlayer(const CPlayer & other)
	: CGameObject(other)
	, m_vStartPos(other.m_vStartPos)
{
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 캐릭터의 행동 상태 초기화
	m_ePreState = PLAYER_IDLE;
	m_eCurState = PLAYER_IDLE;
	m_fTextureSpeed = 15.f;

	// Palette 부여
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	m_pUserInterface = (CPalette_Circle*)pManagement->Get_GameObject_Pointer(*(SCENE_ID*)pArg, __T("Layer_UI"), 1);
	if (nullptr == m_pUserInterface)
		return E_FAIL;
	Safe_AddRef(m_pUserInterface);

	m_pPlayerHand = (CPlayer_Hand*)pManagement->Get_GameObject_Pointer(*(SCENE_ID*)pArg, __T("Layer_Player_Hand"));
	if (nullptr == m_pPlayerHand)
		return E_FAIL;
	Safe_AddRef(m_pPlayerHand);

	//m_pPlayerHand->Set_Player_Scale(m_pTransformCom->Get_TransformDesc().vScale);

	// 카메라 옵저버 세팅
	m_pCam_Observer = new CCamera_Observer;
	pManagement->Subscribe(m_pCam_Observer);
	
	return S_OK;
}

int CPlayer::Update_GameObject(float fTimeDelta)
{
	if (false == m_bIsInit)
	{
		// Ready가 끝나고 색상이랑 위치랑 다 설정 될거라서 레디와 업데이트 사이에서 손 생성
		if (FAILED(Create_Player_Hand()))
			return 0;

		D3DXVECTOR3 vPos = m_pTransformCom->Get_TransformDesc().vPosition;
		vPos.y -= 0.25f;
		vPos.z = m_vStartPos.z - 0.2f;
		m_pTransformCom->Set_Position(vPos);

		if (FAILED(m_pTransformCom->Update_Transform()))
			return 0;

		m_bIsInit = true;
	}
	if (PLAYER_TALK == m_eCurState)
		return 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	D3DXVECTOR3 vPos = m_pTransformCom->Get_TransformDesc().vPosition;
	vPos.z = m_vStartPos.z - 0.2f;
	m_pTransformCom->Set_Position(vPos);
	pManagement->Notify(OBJ_DATA::PLAYER_POSITION, (void*)&vPos);

	//텍스쳐
	m_fTextureIndex += m_fTextureSpeed * fTimeDelta;
	//죽었을때 모든 기능 막음
	//Max texture가 되면 기존을 삭제하고 다시 clone해서 그려지게 됨

	if (m_ePreState == PLAYER_DEAD && m_fMaxTextureIndex <= m_fTextureIndex)
	{
		//죽으면 시작지점으로 초기화
		m_pTransformCom->Set_Position(m_vStartPos);
		m_ePreState = PLAYER_IDLE;
		m_eCurState = PLAYER_IDLE;

		/*사운드*/
		pManagement->Play_Sound(L"a_hue revive.ogg", CSound_Manager::PLAYER_REVIVE);
		pManagement->StopSound(CSound_Manager::COLOR_CLOSE);
		pManagement->StopSound(CSound_Manager::COLOR_OPEN);
		pManagement->StopSound(CSound_Manager::PLAYER_DEAD);
	}
	else if (m_ePreState == PLAYER_DEAD)
	{
		m_pUserInterface->Release_Click();
		return 0;
	}

	if (m_fTextureIndex > m_fMaxTextureIndex)
		m_fTextureIndex = 0.f;

	int iCam_Effect_Num = dynamic_cast<CCamera_Observer*>(m_pCam_Observer)->Get_Cam_Effect();

	if (iCam_Effect_Num == (int)CAM_EFFECT_REVOLUTION_Y)
	{
		m_pUserInterface->Release_Click();
		return 0;
	}
	if (iCam_Effect_Num == (int)CAM_EFFECT_EXPLORER)
		return 0;

	if (FAILED(Movement(fTimeDelta)))
		return 0;	

	if (m_eCurState != PLAYER_LADDER)
		m_pTransformCom->Update_Integrate(fTimeDelta);

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	// 충돌을 위한 현재 플레이어의 위치 및 크기 갱신
	if (FAILED(SetUp_CollisionDesc()))
		return 0;

	if (FAILED(Create_Effect()))
		return 0;
	 
	return 0;
}

int CPlayer::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;
	
	return 0;
}

HRESULT CPlayer::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (PLAYER_TALK == m_ePreState)
	{
		if (FAILED(m_pTextureCom[PLAYER_IDLE]->Set_Texture(int(m_fTextureIndex))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom[m_ePreState]->Set_Texture(int(m_fTextureIndex))))
			return E_FAIL;
	}
	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::Change_Player_State(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (m_ePreState == m_eCurState)
		return;
	switch (m_eCurState)
	{
	case PLAYER_IDLE:
		m_fMaxTextureIndex = 1.f;
		break;
	case PLAYER_WALK:
		m_fMaxTextureIndex = 12.f;
		break;
	case PLAYER_JUMP:
		m_fMaxTextureIndex = 2.f;		
		break;
	case PLAYER_LADDER:
		m_fMaxTextureIndex = 1.f;
		break;
	case PLAYER_DEAD:
		m_fMaxTextureIndex = 12.f;
		// 플레이어 사망 시 이미지 반전
		m_pTransformCom->Set_Scale(D3DXVECTOR3(m_pTransformCom->Get_TransformDesc().vScale.x * -1.f, m_pTransformCom->Get_TransformDesc().vScale.y, m_pTransformCom->Get_TransformDesc().vScale.z));
		m_pTransformCom->ClearVelocity();

		/*사운드*/
		pManagement->Play_Sound(L"a_hue death.ogg", CSound_Manager::PLAYER_DEAD);
		break;
	}
	m_ePreState = m_eCurState;
	m_fTextureIndex = 0.f;
}

HRESULT CPlayer::Key_Input(float _fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	CPlayer_Hand::PLAYER_HAND_STATE eTempState = m_pPlayerHand->Get_Player_Hand_State();
		
#pragma region PLAYER_MOVE
	switch (dynamic_cast<CCamera_Observer*>(m_pCam_Observer)->Get_Cam_View())
	{
	case 0:	//Third Side View
		if (pManagement->KeyPressing('A'))
		{
			if (m_eCurState != PLAYER_JUMP && eTempState != CPlayer_Hand::PLAYER_HAND_LADDER_GRAP)
			{
				m_eCurState = PLAYER_WALK;
				/*사운드*/
				pManagement->Play_Sound(L"a_hue footsteps default.wav", CSound_Manager::PLAYER);
			}

			if ((eTempState == CPlayer_Hand::PLAYER_HAND_BOX_GRAP || eTempState == CPlayer_Hand::PLAYER_HAND_LADDER_GRAP)
				&& m_pTransformCom->Get_TransformDesc().vScale.x > 0.f)		// 손의 상태가 잡기이면 뒤돌지 않도록 한다.
			{
				m_pTransformCom->Set_Scale(D3DXVECTOR3(abs(m_pTransformCom->Get_TransformDesc().vScale.x), m_pTransformCom->Get_TransformDesc().vScale.y, m_pTransformCom->Get_TransformDesc().vScale.z));
				m_pTransformCom->Go_Side(-_fTimeDelta);
				if (m_pPlayerHand->Get_Grap_Box())
					m_pPlayerHand->Move_PushBox(-_fTimeDelta);
			}
			else
			{
				m_pTransformCom->Set_Scale(D3DXVECTOR3(-1.f * abs(m_pTransformCom->Get_TransformDesc().vScale.x), m_pTransformCom->Get_TransformDesc().vScale.y, m_pTransformCom->Get_TransformDesc().vScale.z));
				m_pTransformCom->Go_Side(_fTimeDelta);
				if (m_pPlayerHand->Get_Grap_Box())
					m_pPlayerHand->Move_PushBox(-_fTimeDelta);
			}
		}
		else if (pManagement->KeyPressing('D'))
		{
			if (m_eCurState != PLAYER_JUMP && eTempState != CPlayer_Hand::PLAYER_HAND_LADDER_GRAP)
			{
				m_eCurState = PLAYER_WALK;
				/*사운드*/
				pManagement->Play_Sound(L"a_hue footsteps default.wav", CSound_Manager::PLAYER);
			}

			if ((eTempState == CPlayer_Hand::PLAYER_HAND_BOX_GRAP || eTempState == CPlayer_Hand::PLAYER_HAND_LADDER_GRAP)
				&& m_pTransformCom->Get_TransformDesc().vScale.x < 0.f)		// 손의 상태가 잡기이면 뒤돌지 않도록 한다.
			{
				m_pTransformCom->Set_Scale(D3DXVECTOR3(-1.f * abs(m_pTransformCom->Get_TransformDesc().vScale.x), m_pTransformCom->Get_TransformDesc().vScale.y, m_pTransformCom->Get_TransformDesc().vScale.z));
				m_pTransformCom->Go_Side(-_fTimeDelta);
				if (m_pPlayerHand->Get_Grap_Box())
					m_pPlayerHand->Move_PushBox(_fTimeDelta);
			}
			else
			{
				m_pTransformCom->Set_Scale(D3DXVECTOR3(abs(m_pTransformCom->Get_TransformDesc().vScale.x), m_pTransformCom->Get_TransformDesc().vScale.y, m_pTransformCom->Get_TransformDesc().vScale.z));
				m_pTransformCom->Go_Side(_fTimeDelta);
				if (m_pPlayerHand->Get_Grap_Box())
					m_pPlayerHand->Move_PushBox(_fTimeDelta);
			}
		}

#ifdef _DEBUG
		if (m_eCurState != PLAYER_LADDER)
		{
			if (pManagement->KeyPressing('W'))
			{
				if (m_eCurState != PLAYER_JUMP)
					m_eCurState = PLAYER_WALK;
				m_pTransformCom->Go_Straight(_fTimeDelta);
			}
			else if (pManagement->KeyPressing('S'))
			{
				if (m_eCurState != PLAYER_JUMP)
					m_eCurState = PLAYER_WALK;
				m_eCurState = PLAYER_WALK;
				m_pTransformCom->Go_Straight(-_fTimeDelta);
			}
		}
#endif // _DEBUG

		

		if (pManagement->KeyPressing('E'))
			m_eCurState = PLAYER_DEAD;

		if (pManagement->KeyPressing(VK_SPACE) && m_ePreState != PLAYER_JUMP)
		{
			m_pTransformCom->Jump(_fTimeDelta, JUMPSPEED);
			m_eCurState = PLAYER_JUMP;
			if (m_ePreState == PLAYER_LADDER || CPlayer_Hand::PLAYER_HAND_BOX_GRAP == m_pPlayerHand->Get_Player_Hand_State())
				m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_IDLE);

			/*사운드*/
			pManagement->Play_Sound(L"a_hue jump.ogg", CSound_Manager::PLAYER_JUMP);
		}

		//사다리를 탔을 경우에 적용되도록 함
		if (m_eCurState == PLAYER_LADDER)
		{
			/*사운드*/
			if (eTempState != CPlayer_Hand::PLAYER_HAND_LADDER_GRAP)
				m_eCurState = PLAYER_JUMP;
			if (GetAsyncKeyState('W') & 0x8000)
			{
				m_pTransformCom->Go_Up(_fTimeDelta * 0.5f);
				m_pPlayerHand->UpMove_When_Ladder();
				pManagement->Play_Sound(L"a_hue footsteps Lader.wav", CSound_Manager::LADDER);
			}
			if (GetAsyncKeyState('S') & 0x8000)
			{
				m_pTransformCom->Go_Up(-_fTimeDelta * 0.5f);
				m_pPlayerHand->DownMove_When_Ladder();
				pManagement->Play_Sound(L"a_hue footsteps Lader.wav", CSound_Manager::LADDER);
			}

			/*사운드*/
			pManagement->StopSound(CSound_Manager::PLAYER_JUMP);
			//m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_BACK_GRAP);
		}

		// 위, 아래 이동
		if (GetAsyncKeyState('Z') & 0x8000)
			m_pTransformCom->Go_Up(_fTimeDelta);
		if (GetAsyncKeyState('C') & 0x8000)
			m_pTransformCom->Go_Up(-_fTimeDelta);

		break;

	case 1: //First Person View
			// 마우스 이동 적용해야 함
			// 사다리 탑승 시 별도로 이동시켜야 함
		if (pManagement->KeyPressing('W'))
		{
			m_eCurState = PLAYER_WALK;
			m_pTransformCom->Go_Straight(_fTimeDelta);
		}
		if (pManagement->KeyPressing('S'))
		{
			m_eCurState = PLAYER_WALK;
			m_pTransformCom->Go_Straight(-_fTimeDelta);
		}

		if (pManagement->KeyPressing('D'))
		{
			m_eCurState = PLAYER_WALK;
			m_pTransformCom->Go_Side(_fTimeDelta);
		}
		if (pManagement->KeyPressing('A'))
		{
			m_eCurState = PLAYER_WALK;
			m_pTransformCom->Go_Side(-_fTimeDelta);
		}


		if (GetAsyncKeyState('Q') & 0x8000)
			m_pTransformCom->Rotate_Axis(CTransform::AXIS_Y, -_fTimeDelta);
		if (GetAsyncKeyState('E') & 0x8000)
			m_pTransformCom->Rotate_Axis(CTransform::AXIS_Y, _fTimeDelta);
		break;
	}
#pragma endregion
	if (pManagement->KeyDown(VK_LBUTTON))
		SetCursorPos(WINCX / 2, WINCY / 2);
	else if (pManagement->KeyPressing(VK_LBUTTON))
 		m_pUserInterface->Click();
	else
		m_pUserInterface->Release_Click();

	if (m_eCurState == PLAYER_WALK)
		m_fEffectTime += _fTimeDelta;
	else
		m_fEffectTime = 0.f;

	Change_Player_State(_fTimeDelta);
	if (SCENE_FINAL == pManagement->Get_Current_SceneID())
	{
		if(pManagement->KeyPressing('F'))
			if (dynamic_cast<CNon_Player_Character*>(pManagement->Get_GameObject_Pointer(SCENE_FINAL, __T("Layer_NPC")) )->Is_Able_to_Talk())
			{
				m_eCurState = PLAYER_TALK;
				m_ePreState = PLAYER_TALK;
			}
	}
	if (pManagement->KeyDown('F') && m_pPlayerHand->Get_Player_Hand_State() != CPlayer_Hand::PLAYER_HAND_END)
	{
		// 사다리
		if (eTempState == CPlayer_Hand::PLAYER_HAND_LADDER_IDLE)
		{
			m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_LADDER_GRAP);
			m_eCurState = PLAYER_LADDER;
		}
		//// 푸쉬 박스
		if(eTempState == CPlayer_Hand::PLAYER_HAND_BOX_IDLE || eTempState == CPlayer_Hand::PLAYER_HAND_BOX_GRAP)
			m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_BOX_GRAP);
		//if (m_pPlayerHand->Get_Player_Hand_State() == CPlayer_Hand::PLAYER_HAND_IDLE)
		//	m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_GRAP);
	}
	else if (!pManagement->KeyPressing('F'))
	{
		if (eTempState == CPlayer_Hand::PLAYER_HAND_BOX_GRAP)
			m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_BOX_IDLE);
	}

	return S_OK;
}

void CPlayer::Set_StartPos(const D3DXVECTOR3 & _vPos)
{
	m_vStartPos = _vPos;
}


HRESULT CPlayer::Add_Component()
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Idle",
		L"Com_Texture_Idle", (CComponent**)&m_pTextureCom[PLAYER_IDLE])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Walk",
		L"Com_Texture_Walk", (CComponent**)&m_pTextureCom[PLAYER_WALK])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Jump",
		L"Com_Texture_Jump", (CComponent**)&m_pTextureCom[PLAYER_JUMP])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Ladder",
		L"Com_Texture_Ladder", (CComponent**)&m_pTextureCom[PLAYER_LADDER])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Player_Dead",
		L"Com_Texture_Dead", (CComponent**)&m_pTextureCom[PLAYER_DEAD])))
		return E_FAIL;
#pragma endregion

#pragma region Com_Transform
	//For.Com_Transform
	CTransform::TRANSFORM_DESC TransformInfo;
	TransformInfo.vScale = { 0.5, 0.5, 1.f };
	TransformInfo.vRotate = { 0.f, 0.f, 0.f };
	TransformInfo.fSpeedPerSec = 3.f;
	TransformInfo.fRadianPerSec = D3DXToRadian(90.f);
	TransformInfo.fMass = 2.f;
	TransformInfo.fDamping = 0.9f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform",
		L"Com_Transform", (CComponent**)&m_pTransformCom, &TransformInfo)))
		return E_FAIL;

	//중력값 적용
	m_pTransformCom->Set_Graity(GRAVITY);
#pragma endregion

#pragma region Com_CubeSphere_Collision
	// For.Com_CubeSphere_Collision
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collision_CubeSphere",
		L"Com_Collision_CubeSphere", (CComponent**)&m_pCubeSphereCollisionCom)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CPlayer::Movement(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return E_FAIL;
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (nullptr == m_pCam_Observer)
		return E_FAIL;
	if (m_eCurState == PLAYER_DEAD)
	{
		Change_Player_State(fTimeDelta);
		return S_OK;
	}
	if (m_eCurState == PLAYER_LADDER)
	{
		m_eCurState = PLAYER_LADDER;
		//m_pPlayerHand->Set_Player_Hand_State(CPlayer_Hand::PLAYER_HAND_BACK_GRAP);
		m_pTransformCom->ClearVelocity();
	}
	else if (m_pTransformCom->Get_TransformDesc().vVelocity.y > 0.f)
	{
		m_eCurState = PLAYER_JUMP;
		m_fTextureIndex = 0.f;
	}
	else if (m_pTransformCom->Get_TransformDesc().vVelocity.y < 0.f)
	{
		m_eCurState = PLAYER_JUMP;
		m_fTextureIndex = 1.f;
	}
	else
		m_eCurState = PLAYER_IDLE;
	Key_Input(fTimeDelta);

	Change_Player_State(fTimeDelta);
	return S_OK;
}

HRESULT CPlayer::SetUp_CollisionDesc()
{
	if (nullptr == m_pCubeSphereCollisionCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();
	D3DXVECTOR3 vPos = tTransformDesc.vPosition;
	D3DXVECTOR3 vHalfScale = tTransformDesc.vScale / 2.f;
	if (0 > vHalfScale.x)
		vHalfScale.x = -vHalfScale.x;

	m_pCubeSphereCollisionCom->SetUp_SphereDesc(vPos, vHalfScale.x);

	return S_OK;
}

HRESULT CPlayer::Create_Player_Hand()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	SCENE_ID eCurScene = (SCENE_ID)pManagement->Get_Current_SceneID();

	m_pPlayerHand = (CPlayer_Hand*)pManagement->Get_GameObject_Pointer(eCurScene, L"Layer_Player_Hand");

	if (nullptr == m_pPlayerHand)
		return 0;

	/*if (FAILED(m_pPlayerHand->Set_Player(this)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CPlayer::Create_Effect()
{
	if (m_fEffectTime > 0.5f)
	{
		m_iStoneNum++;
		Ready_Layer_Effect(L"GameObject_Stone_Effect", L"Layer_Stone_Effect", { 0.f, -rand() % 6 * 0.05f, 0.f });

		if (m_iStoneNum > 5)
		{
			m_fEffectTime = 0.f;
			m_iStoneNum = 0;
		}
		else
			m_fEffectTime = 0.47f;
	}
	return S_OK;
}

HRESULT CPlayer::Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, const D3DXVECTOR3 & vPosition)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// 위치만 세팅해서 넘긴다
	CTransform::TRANSFORM_DESC TransformInfo_Effect;
	TransformInfo_Effect.vPosition = vPosition;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, GameObjectTag, SCENE_STAGE, LayerTag, &TransformInfo_Effect)))
		return E_FAIL;
	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);
	
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone_GameObject(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	//
	for(int i = 0; i < PLAYER_END; ++i)
		Safe_Release(m_pTextureCom[i]);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCubeSphereCollisionCom);
	Safe_Release(m_pUserInterface);
	//Unsubscribe(m_pCam_Observer);
	//플레이어에서 Unsubscribe 호출하면 매니저가 살아나므로 보류..
	//옵저버 매니저에서 끊도록 함
	Safe_Release(m_pCam_Observer);
	Safe_Release(m_pPlayerHand);
	//
	CGameObject::Free();
}
