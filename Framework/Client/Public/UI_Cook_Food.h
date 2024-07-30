#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_Food final : public CUI_Cook_WIndow
{
public:
	CUI_Cook_Food();
	CUI_Cook_Food(const CUI_Cook_Food& rhs);
	virtual ~CUI_Cook_Food() = default;

public:
	_bool Get_Selected() { return m_bSelected; }
	void  Set_Selected(_bool bSelected) { m_bSelected = bSelected; }

	_bool Get_CanCooked() { return m_bCanCooked; }
	void  Set_CanCooked(_bool bCanCooked) { m_bCanCooked = bCanCooked; }

	_uint Get_CookCurNum(){ return m_iCooked_CurNum; }
	_uint Get_CookMaxNum() { return m_iCanCooked_MaxNum; }

	void Set_CookCurNum(_uint iOffset) { m_iCooked_CurNum += iOffset; }

	void Set_StartCooking(_bool bStartCooking) { m_bStartCooking = bStartCooking; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool m_bSelected = false;
	_bool m_bCanCooked = false;
	_bool m_bStartCooking = false;

	CUI*  m_pSelectFood = nullptr;

	_uint m_iCanCooked_MaxNum = 0;
	_uint m_iCooked_CurNum = 0;

private:
	void Create_Cook_Window(_float fTimeDelta);
	void Select_Food();
	void SetUp_CookDetail(class CNpc_Cooker* pCooker);
	void SetUp_CookIngrd(class CNpc_Cooker* pCooker);
	void SetUp_CookIngrdNum(class CNpc_Cooker* pCooker, ITEM_TYPE eIngrd1, ITEM_TYPE eIngrd2, _uint iNeedNum1, _uint iNeedNum2 = 0);
	void Calculate_CanCookedNum(_uint iItemNum1, _uint iItemNum2, _uint iNeedNum1, _uint iNeedNum2);
	void Cooking();
	void Add_Food(ITEM_TYPE eIngrd1, ITEM_TYPE eIngrd2, _uint iNeedNum1, _uint iNeedNum2 = 0);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_Food* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END