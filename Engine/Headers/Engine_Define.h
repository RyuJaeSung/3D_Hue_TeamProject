#pragma once

#ifndef __ENGINE_DEFINE_H__

#pragma warning(disable : 4251)

#pragma region STRING

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

#pragma endregion

#pragma region DLL

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#pragma endregion

#pragma region NAMESPACE

#define BEGIN(Name) namespace Name {
#define END }
#define USING(Name) using namespace Name;

#pragma endregion

#pragma region SINGLETON

#define DECLARE_SINGLETON(Class)						\
public:													\
	static Class* Get_Instance();						\
	static DWORD Destroy_Instance();					\
private:												\
	static Class* m_pInstance;
														
#define IMPLEMENT_SINGLETON(Class)					\
Class* Class::m_pInstance = nullptr;				\
Class* Class::Get_Instance()						\
{													\
	if (nullptr == m_pInstance)						\
		m_pInstance = new Class;					\
	return m_pInstance;								\
}													\
DWORD Class::Destroy_Instance()						\
{													\
	DWORD dwRefCnt = 0;								\
	if (m_pInstance)								\
		dwRefCnt = m_pInstance->Release();			\
	if(0 == dwRefCnt)								\
		m_pInstance = nullptr;						\
	return dwRefCnt;								\
}

#pragma endregion

#pragma region MSGBOX

#ifndef _AFX
#define ERR_MSG(message) MessageBox(nullptr, message, L"SystemError", MB_OK)
#else
#define ERR_MSG(message) AfxMessageBox(message)
#endif

#pragma endregion

#pragma region MATHEMATICS

#define PI 3.141592f

#pragma endregion

#define __ENGINE_DEFINE_H__
#endif