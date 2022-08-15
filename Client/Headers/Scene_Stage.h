#pragma once

#ifndef __SCENE_STAGE_H__

#include "Scene.h"

BEGIN(Client)
class CMyObject;
class CScene_Stage final : public CScene
{
public:
	enum TILE_OUTLINE { TILE_LT, TILE_RT, TILE_LB, TILE_RB, TILE_END};

public:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene_Stage() = default;

public:
	// Getter
	const bool& Get_StageKey() const { return m_bStageKey; }

	// Setter
	void Set_StageKey(const bool& _bStageKey) { m_bStageKey = _bStageKey; }
	void Set_NextStage(const bool& _bNextStage) { m_bGoNextStage = _bNextStage; }

public:
	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene(float fTimeDelta) override;
	virtual int LateUpdate_Scene(float fTimeDelta) override;

private:
	HRESULT Ready_Layer_Skybox(const wstring& LayerTag);
	HRESULT Ready_Layer_Player(const wstring& LayerTag);
	HRESULT Ready_Layer_Camera(const wstring& LayerTag);
	HRESULT Ready_Layer_UI(const wstring& LayerTag);
	HRESULT Ready_Layer_Item(const wstring& LayerTag);

	// Ÿ�� ���� �ҷ����� �Լ�
	// �ҷ��� ���ϸ�(Ȯ����X), �ҷ��� �� ���ڷ� �Ѱ��ֱ�
	HRESULT Load_TileData(const wstring& _wstrFileName);
	HRESULT Load_ObjectData(const wstring& _wstrFileName);

public:
	//Getter
	vector<CMyObject*>* Get_OutofStage_Object() { return &m_vecOutofStage; }

public:
	HRESULT Store_Color_Object(CMyObject* _pObject);		// ���ܵ� ���� ������Ʈ�� ��Ƶα� ���� �Լ�
	HRESULT Clear_Color_Object();							// ���� ������ Ȯ���ϸ� ���͸� ���� ���� �Լ�.

public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pDevice);
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pDevice, const SCENE_ID& _eNaxtScene);
	virtual void Free() override;

private:
	SCENE_ID	m_eCurSceneID = SCENE_STAGE;
	int			m_iStageLevel = 0;

	bool		m_bStageKey = false; // ���� ���������� �Ѿ ���踦 ������ �ִ°�? (����� �浹���� ��)
	bool		m_bGoNextStage = false; // ���� ���������� �� �� �ִ°�? (���踦 ������ �ְ� ���̶� �浹���� ��)

	vector<CMyObject*> m_vecOutofStage;

	// ī�޶� ���� ���� ����
	D3DXVECTOR3		m_ArrTilePos[TILE_END];		// ���� ���� �ִ� Ÿ�ϵ��� ��ġ�� ��´�

	D3DXVECTOR3		m_vGoalPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3		m_vKeyPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3		m_vItemPos = { 0.f, 0.f, 0.f };
};

END

#define __SCENE_STAGE_H__
#endif