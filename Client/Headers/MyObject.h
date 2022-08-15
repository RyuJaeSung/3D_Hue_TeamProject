#pragma once

#ifndef __MYOBJECT_H__
#define __MYOBJECT_H__

BEGIN(Client)

// MyObject�� ���� ������� ����غ���
// ��� �浹X
class CMyObject : public CGameObject
{
protected:
	explicit CMyObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CMyObject(const CMyObject& other);
	virtual ~CMyObject() = default;

public:
	// Getter
	const COLOR_TYPE& Get_TextureColor() const { return m_eColorType; }
	const DWORD& Get_Color() const { return m_dwColor; }

	// Setter
	void Set_TextureColor(const COLOR_TYPE& _eColorType);
	void Set_TextureIndex(const int& _iIndex) { m_iTextureIndex = _iIndex; }
	void Set_OutofScreen(bool _bOut) { m_bOutofScreen = _bOut; }

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	HRESULT Render_OutofGameObject();

	//����Ʈ ���̾� �߰�
protected:
	HRESULT Ready_Layer_Effect(const wstring & GameObjectTag, const wstring& LayerTag, CTransform::TRANSFORM_DESC TransformDesc);

protected:
	virtual HRESULT Add_Component();
	virtual HRESULT Collision(float _fTimeDelta);
	HRESULT SetUp_Billboard();

private:
	void SetUp_TextureColor();

protected:
	HRESULT Curling();

public:
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

protected:
	CVIBuffer_CubeTexture*	m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	CFrustum*				m_pFrustumCom = nullptr;
	CObserver*				m_pObserver = nullptr;

	COLOR_TYPE				m_eColorType = COLOR_END; // ������Ʈ ����
	DWORD					m_dwColor = 0;

	int						m_iTextureIndex = 0;
	//�ӵ��� �߷°� �ʱ�ȭ�� ���� bool
	bool					m_IsSetting = true;
	bool					m_bInFrustum = true;

	// ��ü�� ���ܵǾ��� �� �ߺ� ���� ������ ���� ����
	bool					m_bOutofScreen = false;
};

END

#endif // !__LASERGUN_H__


