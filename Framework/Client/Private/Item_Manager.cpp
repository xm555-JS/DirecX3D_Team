#include "stdafx.h"
#include "Item_Manager.h"
#include "UI_Item.h"
#include "UI_Food_Icon.h"

IMPLEMENT_SINGLETON(CItem_Manager)

CItem_Manager::CItem_Manager()
{
}

HRESULT CItem_Manager::Initialize()
{

	return S_OK;
}

void CItem_Manager::Add_Item(ITEM_TYPE eItemType, _uint iAddNum)
{
	auto& iter = m_Items.find(eItemType);

	// 없으면 아이템 추가
	if (iter == m_Items.end())
	{
		m_Items.emplace(eItemType, iAddNum);
		return;
	}

	iter->second += iAddNum;
}

void CItem_Manager::Use_Item(ITEM_TYPE eItemType, _uint iUseNum)
{
	auto& iter = m_Items.find(eItemType);

	if (iter == m_Items.end())
		return ;

	if (0 > iter->second - iUseNum)
		return ;

	iter->second -= iUseNum;

	if (0 == iter->second)
		m_Items.erase(eItemType);

	/*_uint iHealNum = Random(10, 50);
	return iHealNum;*/
}

_uint CItem_Manager::Use_Food()
{
	auto& iter = m_Items.find(m_eCurItem);

	if (iter == m_Items.end())
		return 0;

	if (0 > iter->second - 1)
		return 0;

	iter->second -= 1;

	
	_uint iHealNum;
	switch (iter->first)
	{
	case ITEM_MUSHROOM:
		iHealNum = 1000;
		break;
	case ITEM_CACTUS:
		iHealNum = 1000;
		break;
	case ITEM_BEET:
		iHealNum = 1000;
		break;
	case ITEM_BRACKEN:
		iHealNum = 1000;
		break;
	case ITEM_WHEAT:
		iHealNum = 1000;
		break;
	case ITEM_LETTUCE:
		iHealNum = 1000;
		break;
	case ITEM_SHELL:
		iHealNum = 1500;
		break;
	case ITEM_HERMITCRAB:
		iHealNum = 1500;
		break;	 

	case ITEM_BRACKEN_SOUP:
		iHealNum = 6000;
		break;
	case ITEM_CACTUS_JUICE:
		iHealNum = 4000;
		break;
	case ITEM_MUSHROOM_TOASTING:
		iHealNum = 4000;
		break;
	case ITEM_HERMITCRAB_SOUP:
		iHealNum = 4000;
		break;
	case ITEM_WHEAT_BREAD:
		iHealNum = 4000;
		break;
	case ITEM_LETTUCE_SALAD:
		iHealNum = 4000;
		break;
	case ITEM_SHELL_STEW:
		iHealNum = 6000;
		break;
	case ITEM_BEET_SOUP:
		iHealNum = 4000;
		break;
	default:
		iHealNum = 1000;
		break;
	}


	if (0 == iter->second)
		m_Items.erase(m_eCurItem);

	return iHealNum;
}

_uint CItem_Manager::Find_ItemNum(ITEM_TYPE eItemType)
{
	auto& iter = m_Items.find(eItemType);

	if (iter == m_Items.end())
		return 0;

	return iter->second;
}

void CItem_Manager::SetUp_FoodInven(vector<CUI_Food_Icon*> vecFoodIcon)
{
	_uint iIdx = 0;
	for (auto& pItem : m_Items)
	{		
		if (ITEM_MUSHROOM <= pItem.first)
		{
			vecFoodIcon[iIdx]->Set_iTextureIdx(pItem.first);
			vecFoodIcon[iIdx]->Set_FoodNum(pItem.second);
			++iIdx;
			if (11 < iIdx)
				return;
		}
	}
	for (; iIdx < vecFoodIcon.size(); ++iIdx)
	{
		vecFoodIcon[iIdx]->Set_iTextureIdx(19);
	}
}

void CItem_Manager::SetUp_ItemInven(vector<CUI_Item*> vecItem)
{
	_uint iIdx = 0;
	for (auto& pItem : m_Items)
	{
		vecItem[iIdx]->Set_iTextureIdx(pItem.first);
		vecItem[iIdx]->Set_ItemNum(pItem.second);
		++iIdx;
	}
	for (; iIdx < vecItem.size(); ++iIdx)
	{
		vecItem[iIdx]->Set_iTextureIdx(0);
	}
}

void CItem_Manager::Free()
{
	__super::Free();
}
