#include "stdafx.h"
#include "Player_Observer.h"
#include "Scene_Stage.h"
#include "MyObject.h"
#include "Circle_Effect.h"

USING(Client)

CCircle_Effect::CCircle_Effect(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CCircle_Effect::CCircle_Effect(const CCircle_Effect & _other)
	:CGameObject(_other)
{
}


HRESULT CCircle_Effect::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCircle_Effect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_vEffectScale = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_fEffectSpeed = 0.45f;
	m_fEndSize = 20.f;

	m_pPlayer_Observer = new CPlayer_Observer;
	pManagement->Subscribe(m_pPlayer_Observer);

	return S_OK;
}

int CCircle_Effect::Update_GameObject(float fTimeDelta)
{
	D3DXMATRIX matScale, matTrans;
	if (m_bClick)		// ũ�� ������
	{
		Set_Effect_Size();
		Set_Player_Pos();
		D3DXMatrixScaling(&matScale, m_vEffectScale.x, m_vEffectScale.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vPlayerPos.x, m_vPlayerPos.y, 0.f);
		m_matWorld = matScale *matTrans;
	}
	if (m_bShow&&!m_bClick)		// ũ�� ���ҽ�
	{
		Set_Effect_Size();
		Set_Player_Pos();
		D3DXMatrixScaling(&matScale, m_vEffectScale.x, m_vEffectScale.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vPlayerPos.x, m_vPlayerPos.y, 0.f);
		m_matWorld = matScale * matTrans;
	}
	return 0;
}

int CCircle_Effect::LateUpdate_GameObject(float fTimeDelta)
{
	if (!m_bShow)
		return 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
		return 0;
	return 0;
}

HRESULT CCircle_Effect::Render_GameObject()
{
	Render_Color_Object();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // ���ĺ��� Ȱ��ȭ on
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	//	D3DBLEND_DESTALPHA ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//	D3DBLEND_DESTCOLOR ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//	D3DBLEND_BOTHSRCALPHA ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//	D3DBLEND_BOTHINVSRCALPHA ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//  D3DBLEND_INVDESTALPHA ���� �������� ȸ���� ������ ��ī�̹ڽ��� ������ ǥ�� ����ϱ� �����. ������ ������ ��������.
	//  D3DBLEND_INVDESTALPHA ���� �������� ��������?�� ������ ��ī�̹ڽ��� ������ ǥ�� ����ϱ� �����. ������ ������ ��������.
	//	D3DBLEND_INVSRCALPHA ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//	D3DBLEND_INVSRCCOLOR ���� ���� �� ��� �������
	//	D3DBLEND_INVSRCALPHA ���İ����� �����Ͽ� �� ���� �� ������� ȿ��
	//	...

	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0x505050ff);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE); // Zread�� ���� �ʰڴ�. ������ �����ִ� Z���� ������ �۰� ũ�� ��� ���� ������ �׸��ڴ�.
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Zwrite�� ���� �ʰڴ�. ���� Z���� ������� �����Ƿ� ���Ŀ� �׷��� ����� �� Z���� ���� �� ����(���� ���� �� ���̴�).
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE); // Zread�� ���� �ʰڴ�. ������ �����ִ� Z���� ������ �۰� ũ�� ��� ���� ������ �׸��ڴ�.
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Zwrite�� ���� �ʰڴ�. ���� Z���� ������� �����Ƿ� ���Ŀ� �׷��� ����� �� Z���� ���� �� ����(���� ���� �� ���̴�).

	/*m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);*/
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // ���ĺ��� Ȱ��ȭ off.
	return S_OK;
}

void CCircle_Effect::Set_Effect_Size()
{
	if (m_bClick)
	{
		if (m_vEffectScale.x < m_fEndSize)
		{
			m_vEffectScale.x += m_fEffectSpeed;
			m_vEffectScale.y += m_fEffectSpeed;
		}
		m_bShow = true;
	}
	else
	{
		if (m_vEffectScale.x > 0.f)
		{
			m_vEffectScale.x -= m_fEffectSpeed;
			m_vEffectScale.y -= m_fEffectSpeed;
		}
		else
			m_bShow = false;
	}
}

void CCircle_Effect::Set_Player_Pos()
{
	m_vPlayerPos = dynamic_cast<CPlayer_Observer*>(m_pPlayer_Observer)->Get_Pos();
}

HRESULT CCircle_Effect::Render_Color_Object()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	vector<CMyObject*>* m_pVecObj = dynamic_cast<CScene_Stage*>(pManagement->Get_CurrentScene_Pointer())->Get_OutofStage_Object();

	for (auto& pObj : *m_pVecObj)
		pObj->Render_OutofGameObject();

	return S_OK;
}

HRESULT CCircle_Effect::Add_Component()
{
#pragma region Com_VIBuffer
	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#pragma endregion
#pragma region Com_Texture
	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_CircleEffect",
		L"Com_Texture_Circle", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

CCircle_Effect * CCircle_Effect::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCircle_Effect* pInstance = new CCircle_Effect(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		ERR_MSG(L"Failed To Creating CCircle_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCircle_Effect::Clone_GameObject(void * pArg)
{
	CCircle_Effect* pInstance = new CCircle_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		ERR_MSG(L"Failed To Creating CCircle_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCircle_Effect::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPlayer_Observer);
	CGameObject::Free();
}
