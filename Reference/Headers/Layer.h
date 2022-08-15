#pragma once

#ifndef __LAYER_H__

#include "GameObject.h"

BEGIN(Engine)

class CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	// Getter
	CGameObject* Get_GameObject_Pointer(int iIndex = 0);
	const list<CGameObject*>* Get_GameObject_List();

public:
	HRESULT Add_GameObject_ToLayer(CGameObject* pObject);
	int Update_Layer(float fTimeDelta);
	int LateUpdate_Layer(float fTimeDelta);
	HRESULT Clear_Layer();

private:
	void Clear_NoneAlived_Object();

public:
	static CLayer* Create();
	virtual void Free() override;

private:
	typedef list<CGameObject*>	GAMEOBJECTS; // 게임 오브젝트의 복제본들이 보관됨.
	GAMEOBJECTS m_GameObjects;
};

END

#define __LAYER_H__
#endif
