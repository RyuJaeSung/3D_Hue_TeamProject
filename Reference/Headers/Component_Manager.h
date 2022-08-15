#pragma once

#ifndef __COMPONENT_MANAGER_H__

#pragma region VIBUFFER
#include "VIBuffer_TriColor.h"
#include "ViBuffer_TriCubeTexture.h"
#include "VIBuffer_RectColor.h"
#include "VIBuffer_RectTexture.h"
#include "VIBuffer_TerrainColor.h"
#include "VIBuffer_TerrainTexture.h"
#include "VIBuffer_CubeTexture.h"
#pragma endregion

#pragma region COLLISION
#include "Collision.h"
#include "Collision_Cube.h"
#include "Collision_Sphere.h"
#include "Collision_CubeSphere.h"
#include "Collision_Raycast.h"
#pragma endregion

#include "Transform.h"
#include "Texture.h"
#include "Frustum.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Container(int iSceneCount);
	HRESULT Add_Component_Prototype(int iSceneID, const wstring& ComponentTag, CComponent* pProtoType);
	CComponent* Clone_Component(int iSceneID, const wstring& ComponentTag, void* pArg);
	HRESULT Clear_ForScene(int iSceneID);

public:
	virtual void Free() override;

private:
	typedef unordered_map<wstring, CComponent*>	PROTOTYPES;
	PROTOTYPES* m_pPrototypes = nullptr; // 동적배열
	int			m_iSceneCount = 0;
};

END

#define __COMPONENT_MANAGER_H__
#endif