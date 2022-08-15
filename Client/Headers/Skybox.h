#pragma once

#ifndef __SKYBOX_H__

#include "GameObject.h"

BEGIN(Client)

class CSkybox final : public CGameObject
{
private:
	enum SKYBOX_OPTION
	{
		SKY_BACK,
		SKY_CLOUD,
		SKY_MOUNT,
		SKY_END
	};

private:
	explicit CSkybox(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkybox(const CSkybox& other);
	virtual ~CSkybox() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float fTimeDelta);

public:
	static CSkybox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer_CubeTexture*	m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom[SKY_END];

	COLOR_TYPE				m_eColorType = COLOR_RED; // ������Ʈ ���� >> �ؽ��� �ϳ��ϳ� ���� �ϸ� �̰ɷ� �ؽ��� �ε��� Ȱ��
	DWORD					m_dwColor = 0;

	CObserver*				m_pObserver = nullptr;
};

END

#define __SKYBOX_H__
#endif