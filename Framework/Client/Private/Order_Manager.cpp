#include "stdafx.h"
#include "Order_Manager.h"


IMPLEMENT_SINGLETON(COrder_Manager)

COrder_Manager::COrder_Manager()
{
}

HRESULT COrder_Manager::Initialize()
{
	return S_OK;
}

_uint COrder_Manager::Order_Item()
{
	//IMPL_ONCE(m_bInitWeaponIdx, m_iGet_WeaponIdx = Random(1, 10));
	
	if (m_iGet_WeaponIdx == m_iOrderIdx)
	{
		m_iGet_WeaponIdx += Random(7, 12);
		//Random(ORDER_2GUN, ORDER_CUBE);

		if (!m_bOwn_Cube)
		{
			return ORDER_CUBE;	
		}
		else if (!m_bOwn_2Gun)
		{
			return ORDER_2GUN;
		}
		else if (!m_bOwn_ShieldAxe)
		{
			return ORDER_SHIELDAXE;
		}
		else
		{
			return Random(ORDER_2GUN, ORDER_CUBE);
		}
	}
	else
	{
		++m_iOrderIdx;
		return Random(ORDER_MUSHROOM, ORDER_HERMITCRAB);
	}
}

void COrder_Manager::Free()
{
	__super::Free();
}
