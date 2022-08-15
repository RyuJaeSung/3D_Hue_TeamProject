#include "stdafx.h"
#include "Loading.h"

#pragma region GAMEOBJECT_HEADERS
#include "Skybox.h"
#include "CubeTile.h"
#include "TriCubeTile.h"

#include "LaserGun.h"
#include "Laser.h"
#include "FixBox.h"
#include "MoveBox.h"
#include "PushBox.h"
#include "SpringBox.h"
#include "Balloon.h"
#include "BalloonBox.h"
#include "Scull.h"
#include "Ladder.h"
#include "Rock.h"
#include "Key.h"
#include "Item.h"
#include "Door.h"
#include "Dust_Effect.h"
#include "Stone_Effect.h"
#include "Item_Effect.h"
#include "Laser_Effect.h"
#include "Pet_Item_Effect.h"
#include "Non_Player_Character.h"

#include "Palette_Circle.h"

#pragma endregion

#pragma region COMPONENT_HEADERS

#pragma endregion


USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice, SCENE_ID eNextSceneID)
	: m_pDevice(pDevice)
	, m_eNextSceneID(eNextSceneID)
{
	Safe_AddRef(m_pDevice);
}

bool CLoading::Get_IsFinished() const
{
	return m_IsFinished;
}

HRESULT CLoading::Ready_Loading()
{
	InitializeCriticalSection(&m_CriticalSection);

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);
	if (nullptr == m_hLoadingThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Ready_Stage_Resources()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	// For.GameObject----------------------------------------------------------------------------------
#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_SkyBox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Objects

#pragma region GameObject_LaserGun
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_LaserGun", CLaserGun::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Laser
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Laser", CLaser::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_FixBox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_FixBox", CFixBox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MoveBox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_MoveBox", CMoveBox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_PushBox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_PushBox", CPushBox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_SpringBox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_SpringBox", CSpringBox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Balloon
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Balloon", CBalloon::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_BalloonBox
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_BalloonBox", CBalloonBox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Scull
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Scull", CScull::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Ladder
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Ladder", CLadder::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Rock
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Rock", CRock::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Key
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Key", CKey::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Item", CItem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Door
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Door", CDoor::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


#pragma region GameObject_Dust_Effect
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Dust_Effect", CDust_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Stone_Effect
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Stone_Effect", CStone_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item_Effect
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Item_Effect", CItem_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Laser_Effect
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Laser_Effect", CLaser_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Pet_Item_Effect
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_Pet_Item_Effect", CPet_Item_Effect::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region GameObject_CubeTile
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_CubeTile", CCubeTile::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_TriCubeTile
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_TriCubeTile", CTriCubeTile::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_NPC
	if (FAILED(pManagement->Add_GameObject_Prototype(SCENE_STAGE, L"GameObject_NPC", CNon_Player_Character::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// For.Component-----------------------------------------------------------------------------------

	// For.Object--------------------------------------------
#pragma region Component_Texture_Objects

#pragma region Component_Texture_LaserGun
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_LaserGun", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/LaserGun%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Laser
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Laser", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Laser/Laser%d.dds", 5))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_FixBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_FixBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/FixBox%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_MoveBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_MoveBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/MoveBox%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_PushBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_PushBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/PushBox%d.dds", 2))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SpringBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SpringBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/SpringBox%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Balloon
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Balloon", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Balloon%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_BalloonBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_BalloonBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/BalloonBox%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Scull
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Scull", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Scull%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Ladder
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Ladder", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Ladder%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Ladder
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Rock", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Rock%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Key
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Key", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Key%d.dds", 1))))
		return E_FAIL;
#pragma endregion
#pragma region Component_Texture_Door
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Door", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Door%d.dds", 2))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_RockDead
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_RockDead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/RockDead/RockDead%d.dds", 7))))
		return E_FAIL;
#pragma endregion

#pragma endregion

	// For.Item--------------------------------------------
#pragma region Component_Texture_Item

#pragma region Component_Texture_Item
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Item", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Object/Item%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Item_Glass
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Item_Glass", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Item/Item_Glass.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Item_bg
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Item_Bg", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Item/Item_bg.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Item_UI
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Item_UI", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Item/Item_UI.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma endregion



#pragma region Component_Texture_SkyBox
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SkyBox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/SkyBox/SkyBox%d.dds", 3))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Cube
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Cube", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Map/Cube/Cube%d.dds", 2))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Stairs
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Stairs", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Cube/Map/Stairs/Stairs%d.dds", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Player
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Idle", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Idle/Player_Idle%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Walk", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Walk/Player_Walk%d.png", 12))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Jump", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Jump/Player_Jump%d.png", 2))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Ladder", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Jump/Player_Jump1.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Dead", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Dead/Player_Dead%d.png", 12))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Player_Hand
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Hand", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Hand/Player_Hand%d.png", 2))))
		return E_FAIL;
	/*if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Hand_Back_Idle", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Hand/Player_Hand_Idle%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Player_Hand_Back_Grap", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Player/Hand/Player_Hand_Grap%d.png", 1))))
		return E_FAIL;*/
#pragma endregion

#pragma region Component_Texture_NPC
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_NPC_Body", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Ending/Body.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_NPC_Head", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Ending/Head%d.png", 2))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_NPC_Text", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Ending/Chat%d.png", 3))))
		return E_FAIL;
#pragma endregion

	// For.UI--------------------------------------------
#pragma region Component_Texture_Palette
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Palette", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/UI/Palette%d.png", 1))))
		return E_FAIL;
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_CircleEffect", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/UI/Circle_Effect.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Quest_Window
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Quest_Window", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/UI/Quest_window%d.png", 1))))
		return E_FAIL;
#pragma endregion


	// For.Effect--------------------------------------------
#pragma region Component_Texture_Stone_Effect
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Stone_Effect", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Effect/Stone_Effect/Stone_Effect%d.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Dust_Effect
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Dust_Effect", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Effect/Dust_Effect/Dust_Effect%d.png", 6))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Item_Effect
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Item_Effect", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Effect/Item_Effect/Item_Effect%d.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Pet_Item_Effect
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Pet_Item_Effect", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Effect/Pet_Item_Effect/Pet_Item_Effect.png", 1))))
		return E_FAIL;
#pragma endregion

	// 컬링 컴포넌트
#pragma region Component_Frustom
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Frustum", CFrustum::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	// For.Collision_Part--------------------------------------------
#pragma region Component_Collision_Cube
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Collision_Cube", CCollision_Cube::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Collision_Sphere
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Collision_Sphere", CCollision_Sphere::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Collision_CubeSphere
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Collision_CubeSphere", CCollision_CubeSphere::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Collision_Raycast
	if (FAILED(pManagement->Add_Component_Prototype(SCENE_STAGE, L"Component_Collision_Raycast", CCollision_Raycast::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	m_IsFinished = true;

	return S_OK;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pDevice, SCENE_ID eNextSceneID)
{
	CLoading* pInstance = new CLoading(pDevice, eNextSceneID);

	if (FAILED(pInstance->Ready_Loading()))
	{
		ERR_MSG(L"Failed To Creating CLoading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
}

unsigned CLoading::ThreadMain(void* pParam)
{
	CLoading* pLoading = (CLoading*)pParam;
	if (nullptr == pLoading)
		return 1;

	EnterCriticalSection(&pLoading->m_CriticalSection);

	HRESULT hr = 0;

	switch (pLoading->m_eNextSceneID)
	{
	case SCENE_STATIC:
		break;
	case SCENE_LOGO:
		break;
	case SCENE_STAGE:
		hr = pLoading->Ready_Stage_Resources();
		break;
	case SCENE_END:
		break;
	default:
		break;
	}

	LeaveCriticalSection(&pLoading->m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;
}
