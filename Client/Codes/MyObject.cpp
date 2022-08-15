#include "stdafx.h"
#include "..\Headers\MyObject.h"
#include "Scene_Stage.h"
#include "Player_Observer.h"
#include "Camera.h"

USING(Client)

CMyObject::CMyObject(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CMyObject::CMyObject(const CMyObject & other)
	: CGameObject(other)
{
}

void CMyObject::Set_TextureColor(const COLOR_TYPE & _eColorType)
{
	m_eColorType = _eColorType;
	
	SetUp_TextureColor(); // ���� Ÿ�Կ� ���� DWORD ���� ����
}

HRESULT CMyObject::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMyObject::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_pObserver = new CPlayer_Observer;
	CManagement::Get_Instance()->Subscribe(m_pObserver);

	return S_OK;
}

int CMyObject::Update_GameObject(float fTimeDelta)
{
	if (FAILED(Curling()))
		return 0;

	if (m_dwColor != dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color()
		&& m_bInFrustum)
	{
		if (FAILED(Collision(fTimeDelta)))
			return 0;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CMyObject::LateUpdate_GameObject(float fTimeDelta)
{

	if (m_bInFrustum)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (m_bOutofScreen)
			return 0;
		else if (m_dwColor == dynamic_cast<CPlayer_Observer*>(m_pObserver)->Get_Color() && !m_bOutofScreen)
		{
			// ������ ������ ���� Ȯ���ϸ� ���������� ����Ʈ�� ��� ��� �� ��ü�� ��Ƶε��� ��.
			dynamic_cast<CScene_Stage*>(pManagement->Get_CurrentScene_Pointer())->Store_Color_Object(this);
			return 0;
		}
			

		if (FAILED(pManagement->Add_RenderList(CRenderer::RENDER_ALPHA, this)))
			return 0;
	}

	return 0;
}

HRESULT CMyObject::Render_GameObject()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	// ���� �ٲٴ� �Լ�
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	if (FAILED(m_pTextureCom->Set_Texture(m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	// �� �������
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CMyObject::Render_OutofGameObject()
{
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_TransformDesc().matWorld);

	// ���� �ٲٴ� �Լ�
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	if (FAILED(m_pTextureCom->Set_Texture(m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	// �� �������
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return S_OK;
}

HRESULT CMyObject::Ready_Layer_Effect(const wstring & GameObjectTag, const wstring & LayerTag, CTransform::TRANSFORM_DESC TransformDesc)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	// ��ġ�� �����ؼ� �ѱ��

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, GameObjectTag, pManagement->Get_Current_SceneID(), LayerTag, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyObject::Add_Component()
{
	return S_OK;
}

HRESULT CMyObject::Collision(float _fTimeDelta)
{
	return S_OK;
}

HRESULT CMyObject::SetUp_Billboard()
{
	// ������: ������ ��ü�� �׻� ī�޶� �ٶ󺸰� ����� ���. ex) Ǯ, ����, ������, ����Ʈ
	// ������� ȸ������� ������� �ش� ��ü�� ������Ŀ� ��������ָ� �ȴ�. (������ �ݴ�� ��ü�� ������Ű�� �ȴ�)

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	D3DXMATRIX matView = pCamera->Get_CameraDesc().matView;

	// ��ü�� ������ (ex ����Ʈ)
	D3DXMATRIX matBillAll;
	D3DXMatrixIdentity(&matBillAll);

	// ������� �������� ������ ��� ȸ�������� ���´�.
	memcpy(&matBillAll.m[0][0], &matView.m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&matBillAll.m[1][0], &matView.m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&matBillAll.m[2][0], &matView.m[2][0], sizeof(D3DXVECTOR3));

	// ������� ���Ѵ�.
	D3DXMatrixInverse(&matBillAll, 0, &matBillAll);

	// �̷��� ���� ������� ������� �ڸ��� ���Ѵ�.
	D3DXMATRIX matWorld = m_pTransformCom->Get_TransformDesc().matWorld;
	m_pTransformCom->Set_StateMatrix(matBillAll * matWorld);


	//// Y�� ������ (ex Ǯ, ����, ������)
	//D3DXMATRIX matBillY;
	//D3DXMatrixIdentity(&matBillY);

	//// ������� Y�� ȸ�������� ���´�. _11, _13, _31, _33
	//matBillY._11 = matView._11;
	//matBillY._13 = matView._13;
	//matBillY._31 = matView._31;
	//matBillY._33 = matView._33;

	//// ������� ���Ѵ�.
	//D3DXMatrixInverse(&matBillY, 0, &matBillY);

	//// �̷��� ���� ������� ������� �ڸ��� ���Ѵ�.
	//D3DXMATRIX matWorld = m_pTransformCom->Get_TransformDesc().matWorld;
	//m_pTransformCom->Set_StateMatrix(matBillY * matWorld);

	return S_OK;
}

void CMyObject::SetUp_TextureColor()
{
	switch (m_eColorType)
	{
	case COLOR_BLACK:
		m_dwColor = RGBA_BLACK;
		break;
	case COLOR_RED:
		m_dwColor = RGBA_RED;
		break;
	case COLOR_ORANGE:
		m_dwColor = RGBA_ORANGE;
		break;
	case COLOR_YELLOW:
		m_dwColor = RGBA_YELLOW;
		break;
	case COLOR_GREEN:
		m_dwColor = RGBA_GREEN;
		break;
	case COLOR_BLUE:
		m_dwColor = RGBA_BLUE;
		break;
	case COLOR_NAVY:
		m_dwColor = RGBA_NAVY;
		break;
	case COLOR_PURPLE:
		m_dwColor = RGBA_PURPLE;
		break;
	case COLOR_PINK:
		m_dwColor = RGBA_PINK;
		break;
	case COLOR_WHITE:
		m_dwColor = RGBA_WHITE;
		break;
	}
}

HRESULT CMyObject::Curling()
{
	if (nullptr == m_pFrustumCom
		|| nullptr == m_pTransformCom)
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	CCamera::CAMERA_DESC tCameraDesc = pCamera->Get_CameraDesc();

	//// �������, ����� ������
	//D3DXMATRIX matProjection, matView;
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	m_pFrustumCom->ConstructFrustum(tCameraDesc.fFar, tCameraDesc.matProj, tCameraDesc.matView);

	CTransform::TRANSFORM_DESC tTransformDesc = m_pTransformCom->Get_TransformDesc();

	m_bInFrustum = m_pFrustumCom->Check_Cube(tTransformDesc.vPosition, tTransformDesc.vScale / 2.f);

	return S_OK;
}

CGameObject * CMyObject::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMyObject::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pObserver);

	CGameObject::Free();
}
