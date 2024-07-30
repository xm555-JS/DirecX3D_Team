#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CUI_Item;
class CUI_Food_Icon;

class CItem_Manager : public CBase
{
public:
	DECLARE_SINGLETON(CItem_Manager)

private:
	CItem_Manager();
	virtual ~CItem_Manager() = default;

public:
	void Set_CurItem(ITEM_TYPE eItemType) { m_eCurItem = eItemType; }

	ITEM_TYPE Get_CurItem() { return m_eCurItem; }

public:
	HRESULT		Initialize();
	void		Add_Item(ITEM_TYPE eItemType, _uint iAddNum = 1);
	void		Use_Item(ITEM_TYPE eItemType, _uint iUseNum = 1);
	_uint		Use_Food();
	_uint 		Find_ItemNum(ITEM_TYPE eItemType);

	void		SetUp_FoodInven(vector<CUI_Food_Icon*> vecFoodIcon);
	void		SetUp_ItemInven(vector<CUI_Item*> vecItem);


private:
	map<ITEM_TYPE, _uint>   m_Items;
	ITEM_TYPE				m_eCurItem = ITEM_END;

public:
	virtual void Free();
};

END