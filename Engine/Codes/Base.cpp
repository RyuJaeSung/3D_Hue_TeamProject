#include "Base.h"

USING(Engine)

CBase::CBase()
{
}

DWORD CBase::AddRef()
{
	return ++m_dwRefCnt;
}

DWORD CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();
		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}
