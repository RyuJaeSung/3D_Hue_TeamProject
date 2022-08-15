#pragma once

#ifndef __OBJECT_MANAGER_H__

#include "Layer.h"

BEGIN(Engine)

class CObject_Manager : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	// Getter
	CGameObject* Get_GameObject_Pointer(int iSceneID, const wstring& LayerTag, int iIndex = 0);
	const list<CGameObject*>* Get_GameObject_List(int iSceneID, const wstring& LayerTag);
	CComponent* Get_Component_Pointer(int iSceneID, const wstring& LayerTag, const wstring& ComponentTag, int iObjectIndex = 0);

public:
	HRESULT Reserve_Container(int iSceneCount);
	HRESULT Add_GameObject_Prototype(int iSceneID, const wstring& GameObjectTag, CGameObject* pProtoType);
	HRESULT Add_GameObject_ToLayer(int iSceneID_FromPrototype, const wstring& GameObjectTag, int iSceneID_ToLayer, const wstring& LayerTag, void* pArg);
	HRESULT Clear_ForScene(int iSceneID);
	int Update_Object_Manager(float fTimeDelta);
	int LateUpdate_Object_Manager(float fTimeDelta);

public:
	virtual void Free() override;

private:
	typedef unordered_map<wstring, CGameObject*>	PROTOTYPES; // ���� ������Ʈ�� �������� ������.
	PROTOTYPES* m_pPrototypes = nullptr; // �����迭

	typedef unordered_map<wstring, CLayer*>	LAYERS;
	LAYERS* m_pLayers = nullptr;	// �����迭

	int		m_iSceneCount = 0;
};

END

#define __OBJECT_MANAGER_H__
#endif