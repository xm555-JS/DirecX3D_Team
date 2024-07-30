#include "stdafx.h"
#include "..\Public\Order.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Order_Manager.h"
#include "UI_Order_ItemName.h"

COrder::COrder()
	: CGameObject()
{
}

COrder::COrder(const COrder & rhs)
	: CGameObject(rhs)
{
}

HRESULT COrder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COrder::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));

	m_bTen = *(_bool*)pArg;

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);

	if (m_bTen)
	{
		// 1»Ì¹è°æ
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 5, 500));

		// °ËÀº È­¸é - ½Ã°£Áö³ª¸é »èÁ¦µÊ     FontPos.x·Î 1»ÌÀÎÁö 10»ÌÀÎÁö
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 19, 100, { 0.f,0.f }, nullptr, COrder_Manager::Get_Instance()->Get_TenOrders(m_iOrderIdx), { 10.f,0.f }));

		// Skilp
		m_pUI_SkipBtn = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULTBUTTON), ID_UINT(UI, UI_ORDER_RESULTBUTTON),
			&CUI::UIDESC(1210.f, 38.f, 105.f, 50.f, 0.f, 3, 95));

		++m_iOrderIdx;
	}
	else
	{
		_uint iOneOrder = COrder_Manager::Get_Instance()->Get_OneOrder();
		// 1»Ì¹è°æ
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 5, 500));

		// °ËÀº È­¸é - ½Ã°£Áö³ª¸é »èÁ¦µÊ     FontPos.x·Î 1»ÌÀÎÁö 10»ÌÀÎÁö
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 19, 100, { 0.f,0.f }, nullptr, iOneOrder, { 1.f,0.f }));

		if (ORDER_2GUN == iOneOrder)
		{
			COrder_Manager::Get_Instance()->Set_Own_2Gun(true);
		}
		else if (ORDER_CUBE == iOneOrder)
		{
			COrder_Manager::Get_Instance()->Set_Own_Cube(true);
		}
		else if (ORDER_SHIELDAXE == iOneOrder)
		{
			COrder_Manager::Get_Instance()->Set_Own_ShieldAxe(true);
		}
	}

	return S_OK;
}

HRESULT COrder::Initialize_Pooling(void* pArg)
{
	m_bTen = *(_bool*)pArg;
	
	m_iOrderIdx = 0;
	m_fAccTime = 0.f;
	m_fClickTime = 0.f;
	m_iUI_TenIdx = 0;
	m_bCreate_UI_TenOrders = false;

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);

	if (m_bTen)
	{
		// 1»Ì¹è°æ
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 5, 500));

		// °ËÀº È­¸é - ½Ã°£Áö³ª¸é »èÁ¦µÊ     FontPos.x·Î 1»ÌÀÎÁö 10»ÌÀÎÁö
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 19, 100, { 0.f,0.f }, nullptr, COrder_Manager::Get_Instance()->Get_TenOrders(m_iOrderIdx), { 10.f,0.f }));

		// Skilp
		m_pUI_SkipBtn = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULTBUTTON), ID_UINT(UI, UI_ORDER_RESULTBUTTON),
			&CUI::UIDESC(1210.f, 38.f, 105.f, 50.f, 0.f, 3, 95));

		++m_iOrderIdx;
	}
	else
	{
		// 1»Ì¹è°æ
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 5, 500));

		// °ËÀº È­¸é - ½Ã°£Áö³ª¸é »èÁ¦µÊ     FontPos.x·Î 1»ÌÀÎÁö 10»ÌÀÎÁö
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 19, 100, { 0.f,0.f }, nullptr, COrder_Manager::Get_Instance()->Get_OneOrder(), { 1.f,0.f }));
	}

	return S_OK;
}

void COrder::Tick(_float fTimeDelta)
{	
	USEGAMEINSTANCE;
	MGR(CSound_Manager)->Set_Volume(CSound_Manager::BGM, 0.f);
	if (m_bCreate_UI_TenOrders)
	{
		Create_UI_TenOrders();
		m_bCreate_UI_TenOrders = false;
	}
}

void COrder::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fClickTime += fTimeDelta;

	RECT rcSelect;
	SetRect(&rcSelect, 0.f, 0.f, g_iWinCX, g_iWinCY);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (m_bTen && 2.5f < m_fClickTime && KEY_DOWN(VK_LBUTTON) && 11 > m_iOrderIdx)
		{
			m_fClickTime = 0.f;
			if (10 > m_iOrderIdx)
			{
				Pooling_Objects();
				// 1»Ì¹è°æ
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
					&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 5, 500));

				// °ËÀº È­¸é - ½Ã°£Áö³ª¸é »èÁ¦µÊ     FontPos.x·Î 1»ÌÀÎÁö 10»ÌÀÎÁö
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
					&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 19, 100, { 0.f,0.f }, nullptr, COrder_Manager::Get_Instance()->Get_TenOrders(m_iOrderIdx), { 10.f,0.f }));

				++m_iOrderIdx;
			}
			else if (10 == m_iOrderIdx)
			{
				Pooling_Objects();
				m_bCreate_UI_TenOrders = true;
				pGameInstance->Add_PoolingObject(m_pUI_SkipBtn);
				++m_iOrderIdx;
			}
		}
	}
}

HRESULT COrder::Render()
{
	return S_OK;
}

void COrder::Set_Skip()
{
	USEGAMEINSTANCE;
	Pooling_Objects();
	m_bCreate_UI_TenOrders = true;
	pGameInstance->Add_PoolingObject(m_pUI_SkipBtn);
	m_iOrderIdx = 11;
}

void COrder::Create_UI_TenOrders()
{
	USEGAMEINSTANCE;
	
	// 10»Ì ¹è°æ
	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT), ID_UINT(UI, UI_ORDER_RESULT),
		&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 4, 500));

	MGR(CSound_Manager)->PlaySound(TEXT("UI_Order_TenResult.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);

	for (_uint i = 0; i < 10; ++i)
	{
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_TENRESULT), ID_UINT(UI, UI_ORDER_TENRESULT),
			&CUI::UIDESC(75.f + 125.f * i, 335.f, 122.f, 600.f, 0.f, COrder_Manager::Get_Instance()->Get_TenOrders(i), 400, { 0.f,0.f }, nullptr, i));
	}
}

void COrder::Pooling_Objects()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjects = nullptr;

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULT));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_ITEM));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_ITEMNAME));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}
}

COrder * COrder::Create()
{
	COrder*		pInstance = new COrder();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COrder");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * COrder::Clone(void * pArg)
{
	COrder*		pInstance = new COrder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COrder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COrder::Free()
{
	__super::Free();
}
