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

	// 타일 정보 불러오는 함수
	// 불러올 파일명(확장자X), 불러올 씬 인자로 넘겨주기
	HRESULT Load_TileData(const wstring& _wstrFileName);
	HRESULT Load_ObjectData(const wstring& _wstrFileName);

public:
	//Getter
	vector<CMyObject*>* Get_OutofStage_Object() { return &m_vecOutofStage; }

public:
	HRESULT Store_Color_Object(CMyObject* _pObject);		// 제외된 색상 오브젝트를 담아두기 위한 함수
	HRESULT Clear_Color_Object();							// 색상 변경을 확인하면 벡터를 비우기 위한 함수.

public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pDevice);
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pDevice, const SCENE_ID& _eNaxtScene);
	virtual void Free() override;

private:
	SCENE_ID	m_eCurSceneID = SCENE_STAGE;
	int			m_iStageLevel = 0;

	bool		m_bStageKey = false; // 다음 스테이지로 넘어갈 열쇠를 가지고 있는가? (열쇠랑 충돌했을 때)
	bool		m_bGoNextStage = false; // 다음 스테이지로 갈 수 있는가? (열쇠를 가지고 있고 문이랑 충돌했을 때)

	vector<CMyObject*> m_vecOutofStage;

	// 카메라 락을 위한 변수
	D3DXVECTOR3		m_ArrTilePos[TILE_END];		// 가장 끝에 있는 타일들의 위치를 담는다

	D3DXVECTOR3		m_vGoalPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3		m_vKeyPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3		m_vItemPos = { 0.f, 0.f, 0.f };
};

END

#define __SCENE_STAGE_H__
#endif