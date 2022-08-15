#pragma once
#ifndef __PALETTE_CIRCLE_H__

#include "UserInterface.h"
#include "Circle_Effect.h"

BEGIN(Client)
class CPalette_Circle final : public CUserInterface
{
private:
	explicit CPalette_Circle(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPalette_Circle(const CPalette_Circle& _other);
	virtual ~CPalette_Circle() = default;

public:
	// Getter
	const bool& Get_Show() const { return m_bShow; }

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

public:
	//Setter
	void Click() { m_bClick = true; }
	void Release_Click() { m_bClick = false; }

public:
	virtual HRESULT Add_Component() override;

private:
	void Appear_Palette(float _fTimeDelta);
	void Disppear_Palette(float _fTimeDelta);

public:
	static CPalette_Circle* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
private:
	bool				m_bShow = false;
	bool				m_bClick = false;

	float				m_fEffectSpeed = 0.f;	// 크기 변환 속도
	float				m_fEndSize = 0.f;
	float				m_fSlowRate = 0.f;		// 슬로우 모션 배율

	BYTE				m_bySelect = 0;				// 마우스 드래그로 선택 (빨간색 0 ~ 분홍색 7)
	DWORD				m_dwColor = 0x00000000;
	float				m_fSizeUp = 0.f;			// 선택 시 사이즈 변환 비율

	POINT				m_ptPrePoint;
	POINT				m_ptCurPoint;
	
	CCircle_Effect*		m_pCircle = nullptr;
	CObserver*			m_pPlayer_Observer = nullptr;
};
END
#define __PALETTE_CIRCLE_H__
#endif // !__PALETTE_CIRCLE_H__