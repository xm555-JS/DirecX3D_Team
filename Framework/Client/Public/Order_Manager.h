#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class COrder_Manager : public CBase
{
public:
	DECLARE_SINGLETON(COrder_Manager)

private:
	COrder_Manager();
	virtual ~COrder_Manager() = default;

public:
	_bool Get_Own_Cube() { return m_bOwn_Cube; }
	_bool Get_Own_2Gun() { return m_bOwn_2Gun; }
	_bool Get_Own_ShieldAxe() { return m_bOwn_ShieldAxe; }

	void Set_Own_Cube(_bool bOwn) { m_bOwn_Cube = bOwn; }
	void Set_Own_2Gun(_bool bOwn) { m_bOwn_2Gun = bOwn; }
	void Set_Own_ShieldAxe(_bool bOwn) { m_bOwn_ShieldAxe = bOwn; }

	void Set_OneOrder(ORDER_TYPE eOrderType) { m_eOneOrder = eOrderType; }
	void Set_TenOrder(ORDER_TYPE eOrderType) { m_vecTenOrders.push_back(eOrderType); }

	ORDER_TYPE Get_OneOrder() { return m_eOneOrder; }
	ORDER_TYPE Get_TenOrders(_uint iOrderIdx) { return m_vecTenOrders[iOrderIdx]; }

	void Reset_TenOrderlist() { m_vecTenOrders.clear(); }

public:
	HRESULT		Initialize();	
	_uint		Order_Item();

private:
	DECL_ONCE(m_bInitWeaponIdx);

	_uint m_iOrderIdx = 0;
	_uint m_iGet_WeaponIdx = 0;

	_bool m_bOwn_Cube = false;
	_bool m_bOwn_2Gun = false;
	_bool m_bOwn_ShieldAxe = false;

private:
	ORDER_TYPE		   m_eOneOrder;
	vector<ORDER_TYPE> m_vecTenOrders;

public:
	virtual void Free();
};

END