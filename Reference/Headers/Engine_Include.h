#pragma once

#ifndef __ENGINE_INCLUDE_H__

#pragma region For_Standard
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <tchar.h>

using namespace std;
#pragma endregion


#pragma region For_DirectX
#include <d3d9.h>
#include <d3dx9.h>
#pragma endregion

#pragma region For_User
#include "Engine_Define.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"
#pragma endregion

#pragma region For_Sound
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
#pragma endregion

#define __ENGINE_INCLUDE_H__
#endif