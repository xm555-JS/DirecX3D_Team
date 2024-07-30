#include "..\Public\Base.h"

CBase::CBase()
{
	int a = 10;
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;	
}

unsigned long CBase::Release()
{
	if (0 != m_dwGarbageCnt)
	{
		MSG_BOX("m_dwGarbageCnt is wrong num!!");
		assert(0 == m_dwGarbageCnt);
	}

	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
	{
		return m_dwRefCnt--;
	}
}

void CBase::Free()
{
}
