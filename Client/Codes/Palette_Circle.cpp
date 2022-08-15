#include "stdafx.h"
#include "Player_Observer.h"
#include "Scene_Stage.h"
#include "Palette_Circle.h"

USING(Client)

CPalette_Circle::CPalette_Circle(LPDIRECT3DDEVICE9 _pDevice)
	:CUserInterface(_pDevice)
{
}

CPalette_Circle::CPalette_Circle(const CPalette_Circle & _other)
	:CUserInterface(_other)
{
}

HRESULT CPalette_Circle::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPalette_Circle::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	m_vScale = D3DXVECTOR3(0.f,0.f,0.f);
	m_vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	D3DXMatrixIdentity(&m_matWorld);

	m_dwColor = 0xff505050;
	pManagement->Notify(OBJ_DATA::PLAYER_COLOR, &m_dwColor);
	m_fEffectSpeed = 1000.f;
	m_fEndSize = 256.f;
	m_fSlowRate = 0.3f;

	m_pCircle = (CCircle_Effect*)pManagement->Get_GameObject_Pointer(*(SCENE_ID*)pArg, __T("Layer_UI"));
	if (nullptr == m_pCircle)
		return E_FAIL;
	Safe_AddRef(m_pCircle);

	return S_OK;
}

int CPalette_Circle::Update_GameObject(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	if (m_bClick)
	{
		m_pCircle->Click(m_bClick);
		pManagement->Set_SlowTime(m_fSlowRate);
		Appear_Palette(fTimeDelta / m_fSlowRate);
	}
	else
	{
		dynamic_cast<CScene_Stage*>(pManagement->Get_CurrentScene_Pointer())->Clear_Color_Object();
		m_pCircle->Click(m_bClick);
		m_ptPrePoint.x = 0;
		m_ptPrePoint.y = 0;
		pManagement->Set_SlowTime(1.f);
		Disppear_Palette(fTimeDelta);
	}

	if (m_bShow)
	{
		D3DXMATRIX matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		m_matWorld = matScale * matTrans;
		// 가려진 색상 오브젝트 출력
	}
	else
		D3DXMatrixIdentity(&m_matWorld);

	return 0;
}

int CPalette_Circle::LateUpdate_GameObject(float fTimeDelta)
{
	CUserInterface::LateUpdate_GameObject(fTimeDelta);
	return 0;
}

HRESULT CPalette_Circle::Render_GameObject()
{
	if (m_bShow)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파블렌딩 활성화 on.

		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// 색깔 넣어주기
		DWORD dwColor;
		D3DXMATRIX matScale, matRotate, matTrans;
		for (int i = 0; i < 8; ++i)
		{
			if (m_bySelect == i)
				m_fSizeUp = 1.2f;
			else
				m_fSizeUp = 1.f;
			D3DXMatrixScaling(&matScale, m_vScale.x * m_fSizeUp, m_vScale.y* m_fSizeUp, m_vScale.z);
			D3DXMatrixRotationZ(&matRotate, -D3DXToRadian((45.f * (i + 0.5f))));
			D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
			m_matWorld = matScale * matRotate * matTrans;
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
			m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

			m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			if (FAILED(m_pTextureCom->Set_Texture(0)))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
				return E_FAIL;
		}

		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파블렌딩 활성화 on.
	}
	return S_OK;
}

HRESULT CPalette_Circle::Add_Component()
{
	if (FAILED(CUserInterface::Add_Component()))
		return E_FAIL;

#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Palette",
		L"Com_Texture_Palette", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

void CPalette_Circle::Appear_Palette(float _fTimeDelta)
{
	if (m_ptPrePoint.x == 0 && m_ptPrePoint.y == 0)
		GetCursorPos(&m_ptPrePoint);

	m_pCircle->Click(m_bClick);
	m_bShow = true;

	if (m_vScale.x < m_fEndSize)
	{
		m_vScale.x += _fTimeDelta * m_fEffectSpeed;
		m_vScale.y += _fTimeDelta * m_fEffectSpeed;

		/*사운드*/
		CManagement* pManagement = CManagement::Get_Instance();
		pManagement->StopSound(CSound_Manager::COLOR_CLOSE);
		pManagement->Play_Sound(L"a_hue colour wheel open.ogg", CSound_Manager::COLOR_OPEN);
	}
	else
	{
		m_vScale.x = m_fEndSize;
		m_vScale.y = m_fEndSize * float(WINCX / WINCY);
	}

	GetCursorPos(&m_ptCurPoint);
	D3DXVECTOR3 vRight = { 1.f,0.f,0.f };
	D3DXVECTOR3 vVec = { float(m_ptCurPoint.x - m_ptPrePoint.x), float(m_ptCurPoint.y - m_ptPrePoint.y), 0.f };

	if (D3DXVec3Length(&vVec) > 2.f)
	{
		D3DXVec3Normalize(&vVec, &vVec);
		float fCos = D3DXVec3Dot(&vRight, &vVec);
		float fRad = acosf(fCos);
		float fDegree = D3DXToDegree(fRad);
		if (vVec.y < 0.f)
			fDegree = 360.f - fDegree;
		int iSelect = int((fDegree + 45.f*0.5f) / 45.f);
		//0 ~ 360도를 체크하나 색 칸은 0도부터 시작하지 않으므로 각도 보정을 해줌
		switch (iSelect)
		{
		case 0:
			m_bySelect = 5;
			m_dwColor = RGBA_NAVY;
			break;
		case 1:
			m_bySelect = 6;
			m_dwColor = RGBA_PURPLE;
			break;
		case 2:
			m_bySelect = 7;
			m_dwColor = RGBA_PINK;
			break;
		case 3:
			m_bySelect = 0;
			m_dwColor = RGBA_RED;
			break;
		case 4:
			m_bySelect = 1;
			m_dwColor = RGBA_ORANGE;
			break;
		case 5:
			m_bySelect = 2;
			m_dwColor = RGBA_YELLOW;
			break;
		case 6:
			m_bySelect = 3;
			m_dwColor = RGBA_GREEN;
			break;
		case 7:
			m_bySelect = 4;
			m_dwColor = RGBA_BLUE;
			break;
		case 8:
			m_bySelect = 5;
			m_dwColor = RGBA_NAVY;
			break;
		default:
			break;
		}
	}
	else
	{
		// 일정 거리 이상 멀어지지 않은 경우 선택되지 않음을 표기하기 위한 분기문
		m_bySelect = -1;
	}
}

void CPalette_Circle::Disppear_Palette(float _fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;
	pManagement->Notify(OBJ_DATA::PLAYER_COLOR, &m_dwColor);
	if (m_vScale.x > 0.f)
	{
		m_vScale.x -= _fTimeDelta * m_fEffectSpeed;
		m_vScale.y -= _fTimeDelta * m_fEffectSpeed;

		/*사운드*/
		pManagement->StopSound(CSound_Manager::COLOR_OPEN);
		if (m_bySelect <= 8)
			pManagement->Play_Sound(L"a_hue colorwheel.ogg", CSound_Manager::COLOR_CLOSE);
	}
	else
		m_bShow = false;
}

CPalette_Circle * CPalette_Circle::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CPalette_Circle* pInstance = new CPalette_Circle(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CPalette_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPalette_Circle::Clone_GameObject(void * pArg)
{
	CPalette_Circle* pInstance = new CPalette_Circle(*this); // 복사 생성

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Clone CPalette_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPalette_Circle::Free()
{
	Safe_Release(m_pCircle);
	Safe_Release(m_pPlayer_Observer);
	CUserInterface::Free();
}
