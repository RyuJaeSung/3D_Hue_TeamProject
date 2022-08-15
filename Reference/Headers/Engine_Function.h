#pragma once

#ifndef __ENGINE_FUNCTION_H__

template <typename T>
DWORD Safe_AddRef(T& ptr)
{
	DWORD dwRefCnt = 0;

	if (ptr)
		dwRefCnt = ptr->AddRef();

	return dwRefCnt;
}

template <typename T>
DWORD Safe_Release(T& ptr)
{
	DWORD dwRefCnt = 0;

	if (ptr)
		dwRefCnt = ptr->Release();

	if (0 == dwRefCnt)
		ptr = nullptr;

	return dwRefCnt;
}

template <typename T>
void Safe_Delete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

#define __ENGINE_FUNCTION_H__
#endif