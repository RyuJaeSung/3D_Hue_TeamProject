#include "stdafx.h"
#include "..\Headers\Non_Player_Character.h"
#include "Player.h"
#include "Item_Inven.h"

USING(Client)
CNon_Player_Character::CNon_Player_Character(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CNon_Player_Character::CNon_Player_Character(const CNon_Player_Character & _other)
	:CGameObject(_other)
{
}

HRESULT CNon_Player_Character::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CNon_Player_Character::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMATRIX matScale, matTrans;
	D3DXVECTOR3 vScale = { 6.f, 6.f , 0.f };
	D3DXVECTOR3 vPos = { 28.f, 4.f , 3.f };
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	m_matWorld = matScale * matTrans;

	return S_OK;
}

int CNon_Player_Character::Update_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	if (!m_bAbletoTalk)
	{
		CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Player"));
		if (pPlayer->Get_Pos()->x > m_matWorld._41 - m_matWorld._11*0.5f && pPlayer->Get_Pos()->x < m_matWorld._41 + m_matWorld._11*0.5f)
			m_bAbletoTalk = true;
	}
	else 
	{
		if(NPC_TALK_END == m_ePreTalkState)
			m_eCurTalkState = NPC_TALK_1;
		switch (m_ePreTalkState)
		{
		case CNon_Player_Character::NPC_TALK_1:
			Render_TalkBox(fTimeDelta);
			break;
		case CNon_Player_Character::NPC_TALK_2:
			Render_TalkBox(fTimeDelta);
			break;
		case CNon_Player_Character::NPC_TALK_WAITING:
			Show_Collection(fTimeDelta);
			break;
		}
	}
	Update_Situation();
	Update_State();
	return 0;
}

int CNon_Player_Character::LateUpdate_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;

	return 0;
}

HRESULT CNon_Player_Character::Render_GameObject()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	if (FAILED(m_pTextureCom[NPC_PARTS_BODY]->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	D3DXMATRIX matWorld;
	matWorld = m_matWorld;
	matWorld._11 *= 0.3f;
	matWorld._22 *= 0.3f;
	matWorld._41 -= 0.1f;
	matWorld._42 += 2.8f;

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	if (m_eCurState != NPC_STATE_ANGRY)
	{
		if (FAILED(m_pTextureCom[NPC_PARTS_HEAD]->Set_Texture(0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom[NPC_PARTS_HEAD]->Set_Texture(1)))
			return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (m_eCurTalkState == NPC_TALK_1)
	{
		matWorld._41 += 1.5f;
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		if (FAILED(m_pTextureCom_Text->Set_Texture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;
	}
	else if(m_eCurTalkState == NPC_TALK_2)
	{
		matWorld._41 += 1.5f;
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		if (NPC_STATE_ANGRY == m_eCurState)
		{
			if (FAILED(m_pTextureCom_Text->Set_Texture(1)))
				return E_FAIL;
		}
		else if (NPC_STATE_GOOD == m_eCurState)
		{
			if (FAILED(m_pTextureCom_Text->Set_Texture(2)))
				return E_FAIL;
		}

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;
	}
	else if (m_eCurTalkState == NPC_TALK_WAITING || m_bShowCollection)
	{
		matWorld._11 *= 0.1f;
		matWorld._22 *= 0.1f;
		matWorld._42 -= 1.f;
		matWorld._41 -= 2.25f;
		DWORD dwColor = 0x00000000;
		CItem_Inven* pInven = dynamic_cast<CItem_Inven*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Item_Inven")));
		if (nullptr == pInven)
			return S_OK;
		for (int i = 0; i < 8; ++i)
		{
			if (dynamic_cast<CItem_Inven*>(pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), __T("Layer_Item_Inven")))->Get_ItemState()[i])
			{
				switch (i)
				{
				case 0:
					dwColor = RGBA_RED;
					break;
				case 1:
					dwColor = RGBA_ORANGE;
					break;
				case 2:
					dwColor = RGBA_YELLOW;
					break;
				case 3:
					dwColor = RGBA_GREEN;
					break;
				case 4:
					dwColor = RGBA_BLUE;
					break;
				case 5:
					dwColor = RGBA_NAVY;
					break;
				case 6:
					dwColor = RGBA_PURPLE;
					break;
				case 7:
					dwColor = RGBA_PINK;
					break;
				}

				matWorld._41 += 0.5f;
				m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

				m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
				m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
				m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

				m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
				m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

				if (FAILED(m_pTextureCom_Item->Set_Texture(0)))
					return E_FAIL;
				if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
					return E_FAIL;

				m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
				m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			}
			else
				m_bClear = false;
		}
	}

	return S_OK;
}

HRESULT CNon_Player_Character::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_Texture_NPC", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma region Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_NPC_Head",
		L"Com_Texture_Head", (CComponent**)&m_pTextureCom[NPC_PARTS_HEAD])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_NPC_Body",
		L"Com_Texture_Body", (CComponent**)&m_pTextureCom[NPC_PARTS_BODY])))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_NPC_Text",
		L"Com_Texture_Text1", (CComponent**)&m_pTextureCom_Text)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Item_UI",
		L"Com_Texture", (CComponent**)&m_pTextureCom_Item)))
		return E_FAIL;
	return S_OK;
}

void CNon_Player_Character::Update_Situation()
{
	if (m_ePreTalkState == m_eCurTalkState)
		return;
	switch (m_eCurTalkState)
	{
	case NPC_TALK_1:
		m_eCurState = NPC_STATE_NORMAL;
		m_fTalkTime = 5.f;
		break;
	case NPC_TALK_2:
		m_fTalkTime = 3.f;
		break;
	case NPC_TALK_WAITING:
		m_fTalkTime = 4.f;
		break;
	}
	m_ePreTalkState = m_eCurTalkState;
}

void CNon_Player_Character::Update_State()
{
	if (m_ePreState == m_eCurState)
		return;
	switch (m_eCurState)
	{
	case NPC_STATE_NORMAL:
		break;
	case NPC_STATE_GOOD:
		break;
	case NPC_STATE_ANGRY:
		break;
	}
	m_ePreState = m_eCurState;
}

void CNon_Player_Character::Render_TalkBox(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;
	m_fTalkTime -= fTimeDelta;
	if (NPC_TALK_1 == m_eCurTalkState)
	{
		m_eCurState = NPC_STATE_NORMAL;
		if (m_fTalkTime < 0.f)
		{
			m_eCurState = NPC_STATE_END;
		}
		if (pManagement->KeyDown('F'))
			m_eCurTalkState = NPC_TALK_WAITING;
	}
	else if (NPC_TALK_2 == m_eCurTalkState)
	{
		
		//끝나면 엔딩임
	}
}

void CNon_Player_Character::Show_Collection(float fTimeDelta)
{
	m_fTalkTime -= fTimeDelta;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;
	m_bShowCollection = true;
	// 인형 출력
	if (m_fTalkTime < 0.f)
	{
		// 인형 갯수에 따른 변화
		if(m_bClear)
			m_eCurState = NPC_STATE_GOOD;
		else
			m_eCurState = NPC_STATE_ANGRY;
	
		m_eCurTalkState = NPC_TALK_2;
	}
}

CNon_Player_Character * CNon_Player_Character::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CNon_Player_Character* pInstance = new CNon_Player_Character(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNon_Player_Character::Clone_GameObject(void * pArg)
{
	CNon_Player_Character* pInstance = new CNon_Player_Character(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNon_Player_Character::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (int i = 0; i < NPC_PARTS_END; ++i)
		Safe_Release(m_pTextureCom[i]);
	Safe_Release(m_pTextureCom_Item);
	Safe_Release(m_pTextureCom_Text);
	CGameObject::Free();
}
