#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
public:
	explicit CBase();
	virtual ~CBase() = default;

public:
	DWORD AddRef();
	DWORD Release();

protected:
	virtual void Free() = 0;

protected:
	DWORD m_dwRefCnt = 0;
};

END
