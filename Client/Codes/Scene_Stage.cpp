#include "stdafx.h"
#include "Scene_Stage.h"
#include "Player.h"
#include "Camera.h"
#include "Static_Camera.h"

#include "Tile.h"
#include "MyObject.h"

#include "Balloon.h"
#include "MoveBox.h"
#include "Rock.h"
#include "Door.h"
#include "Item.h"

#include "Item_Inven.h"

USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CScene_Stage::Ready_Scene()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	m_iStageLevel = (int)m_eCurSceneID - (int)SCENE_STAGE;

	if (FAILED(Ready_Layer_Skybox(L"Layer_SkyBox")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;
	if (SCENE_FINAL == m_eCurSceneID)
	{
		if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_NPC", SCENE_FINAL, __T("Layer_NPC"))))
			return E_FAIL;
	}

	TCHAR szStageData[32] = L"";
	SCENE_FINAL == m_eCurSceneID ? wsprintf(szStageData, L"Ending", m_iStageLevel) : wsprintf(szStageData, L"Stage%d", m_iStageLevel);

	if (FAILED(Load_TileData((wstring)szStageData)))
		return E_FAIL;

	if (FAILED(Load_ObjectData((wstring)szStageData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(L"Layer_Item")))
		return E_FAIL;

	/*배경사운드*/
	pManagement->StopSound(CSound_Manager::BGM);

	TCHAR szStageBgmData[32] = L"";
	SCENE_FINAL == m_eCurSceneID ? wsprintf(szStageBgmData, L"Ending.mp3", m_iStageLevel) : wsprintf(szStageBgmData, L"a_hue bgm stage%d.ogg", m_iStageLevel);
	pManagement->Play_BGM(szStageBgmData);
	
	CStatic_Camera* pCamera = (CStatic_Camera*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	pCamera->Change_Camera_Effect(CAM_EFFECT_EXPLORER, pManagement->Get_DeltaTime());

	return S_OK;
}

int CScene_Stage::Update_Scene(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CTransform* pPlayer_Transform = (CTransform*)pManagement->Get_Component_Pointer(pManagement->Get_Current_SceneID(), L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayer_Transform)
		return 0;

#pragma region CHEAT_KEY
	if (pManagement->KeyPressing('K'))
		pPlayer_Transform->Set_Position(m_vKeyPos);

	if (pManagement->KeyPressing('N'))
		pPlayer_Transform->Set_Position(m_vGoalPos);

	if (pManagement->KeyPressing('I'))
		pPlayer_Transform->Set_Position(m_vItemPos);
#pragma endregion

	return 0;
}

int CScene_Stage::LateUpdate_Scene(float fTimeDelta)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (pManagement->KeyPressing('F')
		&& true == m_bGoNextStage)
	{
		/*사운드*/
		pManagement->Play_Sound(L"a_hue bigdoor unlock.wav", CSound_Manager::DOOR);
		pManagement->StopSound(CSound_Manager::COLOR_CLOSE);
		pManagement->StopSound(CSound_Manager::COLOR_OPEN);
		pManagement->StopSound(CSound_Manager::SCULL_SHAKE);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		int iNextScene = (int)m_eCurSceneID + 1;
		/*if (SCENE_FINAL < iNextScene)
		{
			if (FAILED(pManagement->SetUp_CurrentScene(iNextScene, CScene_Stage::Create(m_pDevice, (SCENE_ID)iNextScene))))
				return 0;

			if (FAILED(pManagement->Clear_ForScene(iNextScene - 1)))
				return 0;

			return 0;
		}*/

		CItem_Inven* pItem_Inven = (CItem_Inven*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_Item_Inven");
		if (nullptr == pItem_Inven)
			return 0;

		bool* pGetItem = pItem_Inven->Get_ItemState();

		if (FAILED(pManagement->SetUp_CurrentScene(iNextScene, CScene_Stage::Create(m_pDevice, (SCENE_ID)iNextScene))))
			return 0;

		if (FAILED(pManagement->Clear_ForScene(iNextScene - 1)))
			return 0;

		if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Item_Inven", iNextScene, L"Layer_Item_Inven", pGetItem)))
			return E_FAIL;
	}

	return 0;
}

HRESULT CScene_Stage::Ready_Layer_Skybox(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_SkyBox", m_eCurSceneID, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage::Ready_Layer_Player(const wstring& LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	// 플레이어 손
	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Player_Hand", m_eCurSceneID, __T("Layer_Player_Hand"), &m_eCurSceneID)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Player", m_eCurSceneID, LayerTag, &m_eCurSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage::Ready_Layer_Camera(const wstring& LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera::CAMERA_DESC CameraDesc;
	CameraDesc.vEye = { 0.f, 0.f, -10.f };
	CameraDesc.vAt = { 0.f, 0.f, 0.f };
	CameraDesc.vUp = { 0.f, 1.f, 0.f };
	CameraDesc.fFovY = D3DXToRadian(60.f);
	CameraDesc.fAspect = WINCX / (float)WINCY;
	CameraDesc.fNear = 1.f;
	CameraDesc.fFar = 500.f;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Static_Camera", m_eCurSceneID, LayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage::Ready_Layer_UI(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_CircleEffect", m_eCurSceneID, LayerTag)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Palette", m_eCurSceneID, LayerTag, &m_eCurSceneID)))
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Quest_Window", m_eCurSceneID, LayerTag)))
		return E_FAIL;

	if (SCENE_STAGE1 == m_eCurSceneID)
	{
		if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STATIC, L"GameObject_Item_Inven", m_eCurSceneID, L"Layer_Item_Inven")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CScene_Stage::Ready_Layer_Item(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CItem::ITEM_DESC itemDesc;

	// 스테이지 내의 아이템 정보
	switch (m_eCurSceneID)
	{
	case SCENE_STAGE1:
		itemDesc.vItemPos = { 14.5f, 1.5f, 0.5f };
		itemDesc.eColorType = COLOR_RED;
		break;
	case SCENE_STAGE2:
		itemDesc.vItemPos = { 7.5f, 7.5f, 2.5f };
		itemDesc.eColorType = COLOR_ORANGE;
		break;
	case SCENE_STAGE3:
		itemDesc.vItemPos = { 37.5f, 1.5f, 1.5f };
		itemDesc.eColorType = COLOR_YELLOW;
		break;
	case SCENE_STAGE4:
		itemDesc.vItemPos = { 10.5f, 8.5f, 2.5f };
		itemDesc.eColorType = COLOR_GREEN;
		break;
	case SCENE_STAGE5:
		itemDesc.vItemPos = { 5.5f, 10.5f, 1.5f };
		itemDesc.eColorType = COLOR_BLUE;
		break;
	case SCENE_STAGE6:
		itemDesc.vItemPos = { 5.5f, 5.5f, 2.5f };
		itemDesc.eColorType = COLOR_NAVY;
		break;
	case SCENE_STAGE7:
		itemDesc.vItemPos = { 6.5f, 20.5f, 1.5f };
		itemDesc.eColorType = COLOR_PURPLE;
		break;
	case SCENE_STAGE8:
		itemDesc.vItemPos = { 12.5f, 16.5f, 1.5f };
		itemDesc.eColorType = COLOR_PINK;
		break;
	}

	if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_Item", m_eCurSceneID, LayerTag, &itemDesc)))
		return E_FAIL;

	m_vItemPos = itemDesc.vItemPos;

	return S_OK;
}

HRESULT CScene_Stage::Load_TileData(const wstring & _wstrFileName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	wstring wstrFilePath = L"../Data/TileData/" + _wstrFileName + L".dat";

	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

#pragma region FROM_TOOL_STRUCT
	// 툴에서 설정해둔 타일 옵션
	// 옵션에 따라 CubeTile / TriCubeTile 로 나뉜다.
	enum TILE_OPTION
	{
		TILE_CUBE,
		TILE_TRICUBE,
		TILE_END
	};

	// 툴 관련 구조체 // 받기만 하고 버림
	typedef struct tagTile_ToToolInfo
	{
		D3DXVECTOR3 vScale = { 1.f, 1.f, 1.f };
		DWORD dwIndex = 0;
		float fFloor = 0.f;
	}TILE_TOTOOL_INFO;
#pragma endregion

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;
	DWORD dwStructSize = 0;

	TILE_OPTION ePreTileOption = TILE_END;
	TILE_OPTION eCurTileOption = TILE_END;

	CTile* pTile = nullptr;
	CTransform::TRANSFORM_DESC tDesc = {};
	TILE_TOTOOL_INFO tTileInfo = {};

	TCHAR szTextureTag[MAX_PATH] = L"";
	int iTextureIndex = 0;

	int iCount = 0; // 큐브랑 삼각 큐브 구분

	while (true)
	{
		ReadFile(hFile, &eCurTileOption, sizeof(TILE_OPTION), &dwByte, nullptr); // 타일옵션 불러오기

		ReadFile(hFile, &dwStructSize, sizeof(DWORD), &dwByte, nullptr); // Tool의 TRANSFORM_DESC 구조체 크기
		ReadFile(hFile, &tDesc, dwStructSize, &dwByte, nullptr); // Transform-Desc 구조체 불러오기 >> 클라이언트에서 설정할 때 사용

#pragma region Need_Not_LoadData_ForClient
																 // 필요가 없습니다,,,
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr); // 텍스쳐 문자열 크기
		ReadFile(hFile, szTextureTag, dwStrByte, &dwByte, nullptr); // 텍스쳐 문자열 불러오기
#pragma endregion

		ReadFile(hFile, &iTextureIndex, sizeof(int), &dwByte, nullptr); // 텍스쳐 인덱스

#pragma region Need_Not_LoadData_ForClient
																		//  필요가 없습니다,,,
		ReadFile(hFile, &tTileInfo, sizeof(TILE_TOTOOL_INFO), &dwByte, nullptr); // 툴에서 사용하는 타일 정보(클라이언트에서는 필요없음)
#pragma endregion

		if (0 == dwByte) // 불러온게 없음
			break;

		if (ePreTileOption != eCurTileOption)
		{
			iCount = 0; // 큐브옵션이 바뀜
			ePreTileOption = eCurTileOption;
		}

		// iCount 에 따라 타일 불러와서 값 대입
		switch (eCurTileOption)
		{
		case TILE_CUBE:
			if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_CubeTile", m_eCurSceneID, L"Layer_Cube")))
				break;

			pTile = (CTile*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_Cube", iCount);
			break;
		case TILE_TRICUBE:
			if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, L"GameObject_TriCubeTile", m_eCurSceneID, L"Layer_TriCube")))
				break;

			pTile = (CTile*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_TriCube", iCount);
			break;
		}

		if (nullptr == pTile)
			break;

		// 텍스쳐 인덱스 설정
		pTile->Set_TextureIndex(iTextureIndex);

		// Transform 설정(위치, 크기, 회전,,,)
		CTransform* pTransform = (CTransform*)pTile->Find_Component(L"Com_Transform");
		if (nullptr == pTransform)
			break;

		pTransform->Set_TransformDesc(tDesc);

		// 가장 끝에 있는 타일들의 위치를 받아온다
		if (iCount == 0)
		{
			m_ArrTilePos[TILE_LT] = tDesc.vPosition;
			m_ArrTilePos[TILE_LB] = tDesc.vPosition;
			m_ArrTilePos[TILE_RT] = tDesc.vPosition;
			m_ArrTilePos[TILE_RB] = tDesc.vPosition;
		}
		else
		{
			if (tDesc.vPosition.x > m_ArrTilePos[TILE_RT].x)
			{
				m_ArrTilePos[TILE_RT].x = tDesc.vPosition.x;
				m_ArrTilePos[TILE_RB].x = tDesc.vPosition.x;
			}
			if (tDesc.vPosition.x < m_ArrTilePos[TILE_LT].x)
			{
				m_ArrTilePos[TILE_LT].x = tDesc.vPosition.x;
				m_ArrTilePos[TILE_LB].x = tDesc.vPosition.x;
			}

			if (tDesc.vPosition.y > m_ArrTilePos[TILE_LT].y)
			{
				m_ArrTilePos[TILE_LT].y = tDesc.vPosition.y;
				m_ArrTilePos[TILE_RT].y = tDesc.vPosition.y;
			}
			if (tDesc.vPosition.y < m_ArrTilePos[TILE_LB].y)
			{
				m_ArrTilePos[TILE_LB].y = tDesc.vPosition.y;
				m_ArrTilePos[TILE_RB].y = tDesc.vPosition.y;
			}
		}

		++iCount;
	}
	CloseHandle(hFile);

	CStatic_Camera* pCamera = (CStatic_Camera*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_Camera");
	pCamera->Set_Cam_OutLine(m_ArrTilePos);		// 카메라에 가장 끝 타일 위치를 넘겨준다

	return S_OK;
}

HRESULT CScene_Stage::Load_ObjectData(const wstring & _wstrFileName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	wstring wstrFilePath = L"../Data/ObjData/" + _wstrFileName + L".dat";

	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

#pragma region FROM_TOOL_STRUCT
	// 툴에서 설정해둔 오브젝트 타입
	enum OBJECT_TYPE
	{
		// Cube
		OBJ_FIXBOX, // 멈춰있는 상자
		OBJ_MOVEBOX, // 혼자서 움직이는 상자
		OBJ_PUSHBOX, // 플레이어가 움직이는 상자
		OBJ_SPRINGBOX, // 닿으면 튀는 상자
		OBJ_SCULL, // 위에서 떨어지는 해골
		OBJ_LADDER, // 사다리
		OBJ_LASERGUN, // 레이저 발사하는 총 // 오브젝트는 검정색 > 가지고 있는 색상 값으로 클라이언트에서 레이저 색상 설정
		OBJ_ROCK, // 굴러떨어지는 돌
		OBJ_KEY, // 문(스테이지 이동)
		OBJ_DOOR, // 문(스테이지 이동)
		OBJ_BALLOON, // 풍선
		OBJ_END
	};

	// 툴 관련 구조체 // 받기만 하고 버림
	typedef struct tagTile_ToToolInfo
	{
		D3DXVECTOR3 vScale = { 1.f, 1.f, 1.f };
		DWORD dwIndex = 0;
		float fFloor = 0.f;
	}TILE_TOTOOL_INFO;
#pragma endregion


	DWORD dwByte = 0;
	DWORD dwStructSize = 0;

	CMyObject* pObject = nullptr;

	OBJECT_TYPE eObjType = OBJ_END;
	COLOR_TYPE eColorType = COLOR_END;
	CTransform::TRANSFORM_DESC tDesc = {};
	TILE_TOTOOL_INFO tTileInfo = {};

	// 추가사항
	COLOR_TYPE eBalloonBox_Color = COLOR_END;
	CMoveBox::DIR_OPTION eMoveBox_Dir = CMoveBox::DIR_END;
	float fMoveBox_Dis = 0.f;

	TCHAR szTextureTag[MAX_PATH] = L"";
	int iTextureIndex = 0;

	D3DXVECTOR3 vMovePos = { 0.f, 0.f, 0.f };

	int iCount = 0;
	int iDoorCount = 0;

	while (true)
	{
		ReadFile(hFile, &eObjType, sizeof(OBJECT_TYPE), &dwByte, nullptr); // 오브젝트 타입 불러오기
		ReadFile(hFile, &eColorType, sizeof(COLOR_TYPE), &dwByte, nullptr); // 색상 불러오기

		ReadFile(hFile, &dwStructSize, sizeof(DWORD), &dwByte, nullptr); // TRANSFORM_DESC 구조체 크기
		ReadFile(hFile, &tDesc, dwStructSize, &dwByte, nullptr); // Transform-Desc 구조체 불러오기

		ReadFile(hFile, &iTextureIndex, sizeof(int), &dwByte, nullptr); // 텍스쳐 인덱스

#pragma region Need_Not_LoadData_ForClient
		ReadFile(hFile, &tTileInfo, sizeof(TILE_TOTOOL_INFO), &dwByte, nullptr); // 툴에서 사용하는 타일 정보(클라이언트에서는 필요없음)
#pragma endregion

		ReadFile(hFile, &eBalloonBox_Color, sizeof(COLOR_TYPE), &dwByte, nullptr); // 풍선의 상자 색상 불러오기
		ReadFile(hFile, &eMoveBox_Dir, sizeof(CMoveBox::DIR_OPTION), &dwByte, nullptr); // 움직이는 상자 방향 불러오기
		ReadFile(hFile, &fMoveBox_Dis, sizeof(float), &dwByte, nullptr); // 움직이는 상자 이동거리 불러오기

#pragma region Need_Not_LoadData_ForClient
		ReadFile(hFile, &vMovePos, sizeof(D3DXVECTOR3), &dwByte, nullptr); // 중심에서 움직인 거리 > 클라에서 안씀
#pragma endregion

		//if (OBJ_ROCK == eObjType)
		//	continue;

		if (0 == dwByte) // 불러온게 없음
			break;

		wstring wstrGameObjectTag = L"";
		wstring wstrLayerTag = L"";

		switch (eObjType)
		{
		case OBJ_FIXBOX:
			wstrGameObjectTag = L"GameObject_FixBox";
			wstrLayerTag = L"Layer_FixBox";
			break;
		case OBJ_MOVEBOX:
			wstrGameObjectTag = L"GameObject_MoveBox";
			wstrLayerTag = L"Layer_MoveBox";
			break;
		case OBJ_PUSHBOX:
			wstrGameObjectTag = L"GameObject_PushBox";
			wstrLayerTag = L"Layer_PushBox";
			break;
		case OBJ_SPRINGBOX:
			wstrGameObjectTag = L"GameObject_SpringBox";
			wstrLayerTag = L"Layer_SpringBox";
			break;
		case OBJ_SCULL:
			wstrGameObjectTag = L"GameObject_Scull";
			wstrLayerTag = L"Layer_Scull";
			break;
		case OBJ_LADDER:
			wstrGameObjectTag = L"GameObject_Ladder";
			wstrLayerTag = L"Layer_Ladder";
			break;
		case OBJ_LASERGUN:
			wstrGameObjectTag = L"GameObject_LaserGun";
			wstrLayerTag = L"Layer_LaserGun";
			break;
		case OBJ_ROCK:
			wstrGameObjectTag = L"GameObject_Rock";
			wstrLayerTag = L"Layer_Rock";
			break;
		case OBJ_KEY:
			wstrGameObjectTag = L"GameObject_Key";
			wstrLayerTag = L"Layer_Key";
			break;
		case OBJ_DOOR:
			wstrGameObjectTag = L"GameObject_Door";
			wstrLayerTag = L"Layer_Door";
			break;
		case OBJ_BALLOON:
			wstrGameObjectTag = L"GameObject_Balloon";
			wstrLayerTag = L"Layer_Balloon";
			break;
		}

		if (FAILED(pManagement->Add_GameObject_ToLayer(SCENE_STAGE, wstrGameObjectTag,
			m_eCurSceneID, wstrLayerTag)))
			break;

		iCount = pManagement->Get_Layer_Size(m_eCurSceneID, wstrLayerTag);
		pObject = (CMyObject*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, wstrLayerTag, iCount - 1);

		if (nullptr == pObject)
			break;

		pObject->Set_TextureIndex(iTextureIndex); // 텍스쳐 인덱스 설정
		pObject->Set_TextureColor(eColorType); // 색상설정

											   // Transform 설정(위치, 크기, 회전,,,)
		CTransform* pTransform = (CTransform*)pObject->Find_Component(L"Com_Transform");
		if (nullptr == pTransform)
			break;

		pTransform->Set_TransformDesc(tDesc);

		// 추가사항 설정
		if (OBJ_BALLOON == eObjType)
			dynamic_cast<CBalloon*>(pObject)->Set_BalloonBox_Color(eBalloonBox_Color);

		if (OBJ_MOVEBOX == eObjType)
		{
			dynamic_cast<CMoveBox*>(pObject)->Set_BoxDir(eMoveBox_Dir); // 박스 이동방향
			dynamic_cast<CMoveBox*>(pObject)->Set_StartPos(pTransform->Get_TransformDesc().vPosition); // 시작 위치
			dynamic_cast<CMoveBox*>(pObject)->Set_Move_Distance(fMoveBox_Dis);
		}

		if (OBJ_KEY == eObjType)
			m_vKeyPos = pTransform->Get_TransformDesc().vPosition;

		if (OBJ_DOOR == eObjType)
		{
			if (0 == iDoorCount)
			{
				CPlayer* pPlayer = (CPlayer*)pManagement->Get_GameObject_Pointer(m_eCurSceneID, L"Layer_Player");
				if (nullptr == pPlayer)
					continue;

				CTransform* pPlayerTransform = (CTransform*)pPlayer->Find_Component(L"Com_Transform");
				if (nullptr == pPlayerTransform)
					continue;

				pPlayer->Set_StartPos(pTransform->Get_TransformDesc().vPosition);
				pPlayerTransform->Set_Position(pTransform->Get_TransformDesc().vPosition);

				pObject->Set_TextureIndex(1); // 열린 문으로 바꿔줌
				dynamic_cast<CDoor*>(pObject)->Set_DoorType(CDoor::DOOR_PREV);
			}
			else
			{
				dynamic_cast<CDoor*>(pObject)->Set_DoorType(CDoor::DOOR_NEXT);
				m_vGoalPos = pTransform->Get_TransformDesc().vPosition;
				m_vGoalPos.z -= 0.2f;
			}

			++iDoorCount;
		}

		if (OBJ_ROCK == eObjType)
		{
			dynamic_cast<CRock*>(pObject)->Set_StartPos(pTransform->Get_TransformDesc().vPosition); // 시작 위치
			dynamic_cast<CRock*>(pObject)->Set_StartColor(eColorType);
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage::Store_Color_Object(CMyObject * _pObject)
{
	if (nullptr == _pObject)
	{
		ERR_MSG(__T("스테이지에서 색상 구분할 때 임시로 제외할 객체가 nullptr."));
		return E_FAIL;
	}
	m_vecOutofStage.push_back(_pObject);
	_pObject->Set_OutofScreen(true);
	Safe_AddRef(_pObject);
	return S_OK;
}

HRESULT CScene_Stage::Clear_Color_Object()
{

	for (size_t i = 0; i < m_vecOutofStage.size(); ++i)
	{
		m_vecOutofStage[i]->Set_OutofScreen(false);
		Safe_Release(m_vecOutofStage[i]);
	}
	m_vecOutofStage.clear();
	m_vecOutofStage.swap(vector<CMyObject*>());
#ifdef _DEBUG
	if (0 != m_vecOutofStage.size())
		ERR_MSG(__T("안 비웠대요~ 망했대요~"));
#endif // _DEBUG

	return S_OK;
}

CScene_Stage* CScene_Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CScene_Stage* pInstance = new CScene_Stage(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		ERR_MSG(L"Failed To Creating CScene_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pDevice, const SCENE_ID & _eNaxtScene)
{
	if (nullptr == pDevice)
		return nullptr;
	CScene_Stage* pInstance = new CScene_Stage(pDevice);
	pInstance->m_eCurSceneID = _eNaxtScene;

	if (FAILED(pInstance->Ready_Scene()))
	{
		ERR_MSG(L"Failed To Creating CScene_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage::Free()
{
	for (size_t i = 0; i < m_vecOutofStage.size(); ++i)
		Safe_Release(m_vecOutofStage[i]);
	m_vecOutofStage.clear();
	m_vecOutofStage.swap(vector<CMyObject*>());
	CScene::Free();
}

