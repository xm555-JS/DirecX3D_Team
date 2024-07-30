#include "stdafx.h"
#include "..\Public\UI_Cook_Food.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Npc_Cooker.h"
#include "Item_Manager.h"


CUI_Cook_Food::CUI_Cook_Food()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_Food::CUI_Cook_Food(const CUI_Cook_Food & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_Food::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_Food::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	USEGAMEINSTANCE;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	m_pSelectFood = (CUI*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_COOK_FOODSELECT));

	SetUp_UIpos();


	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Cook_Food::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;
	m_bSelected = false;

	USEGAMEINSTANCE;
	
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	m_pSelectFood = (CUI*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_COOK_FOODSELECT));

	SetUp_UIpos();


	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Cook_Food::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Create_Cook_Window(fTimeDelta);
	Select_Food();

	CNpc_Cooker* pCooker = (CNpc_Cooker*)GAMEINSTANCE->Get_Object(m_iCurLevel, ID_UINT(NPC, NPC_COOKER));
	// 선택되면
	if (m_bSelected)
	{
		SetUp_CookDetail(pCooker);
		SetUp_CookIngrd(pCooker);
	}

	if (m_bStartCooking)
	{
		Cooking();
		SetUp_CookIngrd(pCooker);
		if (m_bCanCooked)
		{
			m_iCooked_CurNum = 1;
		}
		else
		{
			m_iCooked_CurNum = 0;
		}
		m_bStartCooking = false;
	}
}

void CUI_Cook_Food::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_Food::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Cook_Food::Create_Cook_Window(_float fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	if (0.5f < m_fAlphaTime)
	{
		m_fAlphaTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, m_fAlphaTime, 0.5f));
	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Cook_Food::Select_Food()
{
	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	CNpc_Cooker* pCooker = (CNpc_Cooker*)GAMEINSTANCE->Get_Object(m_iCurLevel, ID_UINT(NPC, NPC_COOKER));

	CUI* pUI = nullptr;
	pUI = pCooker->Get_UI_CookName();
	if (PtInRect(&rcSelect, ptMouse))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
			list<CGameObject*>* pGameObjects = nullptr;
			pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_FOOD));
			for (auto& elem : *pGameObjects)
			{
				((CUI_Cook_Food*)elem)->Set_Selected(false);
				((CUI_Cook_Food*)elem)->Set_CanCooked(false);
			}
			m_bSelected = true;
			m_pSelectFood->Set_vUIPos({ m_fX,m_fY });
			SetUp_CookDetail(pCooker);
			SetUp_CookIngrd(pCooker);

			
			pUI->Set_bRenderFontNum(true);
			if (m_bCanCooked)
			{
				m_iCooked_CurNum = 1;
			}
			else
			{
				m_iCooked_CurNum = 0;
			}

		}
	}
	if (m_bSelected)
	{
		pUI->Set_FontNum(m_iCooked_CurNum);
	}
}

void CUI_Cook_Food::SetUp_CookDetail(CNpc_Cooker* pCooker)
{	
	CUI* pUI = nullptr;
	pUI = pCooker->Get_UI_CookName();
	pUI->Set_iTextureIdx(m_tUIDesc.iTextureIdx - 8);
	pUI = pCooker->Get_UI_CookDetail();
	pUI->Set_iTextureIdx(m_tUIDesc.iTextureIdx);
}

void CUI_Cook_Food::SetUp_CookIngrd(CNpc_Cooker* pCooker)
{
	switch (m_tUIDesc.iTextureIdx)
	{
	case ITEM_BRACKEN_SOUP:
		SetUp_CookIngrdNum(pCooker, ITEM_BRACKEN, ITEM_LETTUCE, 2, 4);
		break;

	case ITEM_CACTUS_JUICE:
		SetUp_CookIngrdNum(pCooker, ITEM_CACTUS, ITEM_END, 2);
		break;

	case ITEM_MUSHROOM_TOASTING:
		SetUp_CookIngrdNum(pCooker, ITEM_MUSHROOM, ITEM_END, 2);
		break;

	case ITEM_HERMITCRAB_SOUP:
		SetUp_CookIngrdNum(pCooker, ITEM_HERMITCRAB, ITEM_MUSHROOM, 2, 4);
		break;

	case ITEM_WHEAT_BREAD:
		SetUp_CookIngrdNum(pCooker, ITEM_WHEAT, ITEM_END, 2);
		break;

	case ITEM_LETTUCE_SALAD:
		SetUp_CookIngrdNum(pCooker, ITEM_LETTUCE, ITEM_END, 2);	
		break;

	case ITEM_SHELL_STEW:
		SetUp_CookIngrdNum(pCooker, ITEM_SHELL, ITEM_LETTUCE, 2, 3);
		break;

	case ITEM_BEET_SOUP:
		SetUp_CookIngrdNum(pCooker, ITEM_BEET, ITEM_END, 2);
		break;
	}
}

void CUI_Cook_Food::SetUp_CookIngrdNum(CNpc_Cooker* pCooker, ITEM_TYPE eIngrd1, ITEM_TYPE eIngrd2, _uint iNeedNum1, _uint iNeedNum2)
{
	CUI* pUI = nullptr;
	_uint iItemNum1 = 0;
	_uint iItemNum2 = 0;
	// Set Ingredient
	pUI = pCooker->Get_UI_Ingrd1();
	pUI->Set_iTextureIdx(eIngrd1);
	pUI = pCooker->Get_UI_Ingrd2();
	pUI->Set_iTextureIdx(eIngrd2);

	// Ingrd1
	iItemNum1 = CItem_Manager::Get_Instance()->Find_ItemNum(eIngrd1);

	if (iItemNum1 >= iNeedNum1)
	{
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::TENSNUM1);
		if (10 < iItemNum1)
		{
			pUI->Set_iTextureIdx(iItemNum1 / 10);
		}
		else
		{
			pUI->Set_iTextureIdx(22);
		}
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::UNITSNUM1);
		pUI->Set_iTextureIdx(iItemNum1 % 10);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::NEEDNUM1);
		pUI->Set_iTextureIdx(iNeedNum1);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::SLASHNUM1);
		pUI->Set_iTextureIdx(20);
	}
	else
	{
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::TENSNUM1);
		pUI->Set_iTextureIdx(22);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::UNITSNUM1);
		pUI->Set_iTextureIdx(iItemNum1 + 10);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::NEEDNUM1);
		pUI->Set_iTextureIdx(iNeedNum1 + 10);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::SLASHNUM1);
		pUI->Set_iTextureIdx(21);
	}

	// Ingrd2
	if (eIngrd2 == ITEM_END)
	{
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::TENSNUM2);
		pUI->Set_iTextureIdx(22);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::UNITSNUM2);
		pUI->Set_iTextureIdx(22);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::NEEDNUM2);
		pUI->Set_iTextureIdx(22);
		pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::SLASHNUM2);
		pUI->Set_iTextureIdx(22);
		iItemNum2 = 0.f;
	}
	else
	{
		iItemNum2 = CItem_Manager::Get_Instance()->Find_ItemNum(eIngrd2);
		if (iItemNum2 >= iNeedNum2)
		{
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::TENSNUM2);
			if (10 < iItemNum2)
			{
				pUI->Set_iTextureIdx(iItemNum2 / 10);
			}
			else
			{
				pUI->Set_iTextureIdx(22);
			}
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::UNITSNUM2);
			pUI->Set_iTextureIdx(iItemNum2 % 10);
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::NEEDNUM2);
			pUI->Set_iTextureIdx(iNeedNum2);
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::SLASHNUM2);
			pUI->Set_iTextureIdx(20);
		}
		else
		{
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::TENSNUM2);
			pUI->Set_iTextureIdx(22);
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::UNITSNUM2);
			pUI->Set_iTextureIdx(iItemNum2 + 10);
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::NEEDNUM2);
			pUI->Set_iTextureIdx(iNeedNum2 + 10);
			pUI = pCooker->Get_UI_IngrdNum(CNpc_Cooker::SLASHNUM2);
			pUI->Set_iTextureIdx(21);
		}
	}
		Calculate_CanCookedNum(iItemNum1, iItemNum2, iNeedNum1, iNeedNum2);
}

void CUI_Cook_Food::Calculate_CanCookedNum(_uint iItemNum1, _uint iItemNum2, _uint iNeedNum1, _uint iNeedNum2)
{
	//m_iCanCooked_MaxNum
	_uint iTemp = iItemNum1 / iNeedNum1;
	_uint iSour;
	if (0 == iNeedNum2)
	{
		m_iCanCooked_MaxNum = iTemp;
	}
	else
	{
		iSour = iItemNum2 / iNeedNum2;
		if (iTemp <= iSour)
		{
			m_iCanCooked_MaxNum = iTemp;
		}
		else
		{
			m_iCanCooked_MaxNum = iSour;
		}
	}

	if (0 == m_iCanCooked_MaxNum)
	{
		m_bCanCooked = false;
	}
	else
	{
		m_bCanCooked = true;
	}
}

void CUI_Cook_Food::Cooking()
{
	switch (m_tUIDesc.iTextureIdx)
	{
	case ITEM_BRACKEN_SOUP:
		Add_Food(ITEM_BRACKEN, ITEM_LETTUCE, 2, 4);
		break;

	case ITEM_CACTUS_JUICE:
		Add_Food(ITEM_CACTUS, ITEM_END, 2);
		break;

	case ITEM_MUSHROOM_TOASTING:
		Add_Food(ITEM_MUSHROOM, ITEM_END, 2);
		break;

	case ITEM_HERMITCRAB_SOUP:
		Add_Food(ITEM_HERMITCRAB, ITEM_MUSHROOM, 2, 4);
		break;

	case ITEM_WHEAT_BREAD:
		Add_Food(ITEM_WHEAT, ITEM_END, 2);
		break;

	case ITEM_LETTUCE_SALAD:
		Add_Food(ITEM_LETTUCE, ITEM_END, 2);
		break;

	case ITEM_SHELL_STEW:
		Add_Food(ITEM_SHELL, ITEM_LETTUCE, 2, 3);
		break;

	case ITEM_BEET_SOUP:
		Add_Food(ITEM_BEET, ITEM_END, 2);
		break;
	}
}

void CUI_Cook_Food::Add_Food(ITEM_TYPE eIngrd1, ITEM_TYPE eIngrd2, _uint iNeedNum1, _uint iNeedNum2)
{
	CItem_Manager::Get_Instance()->Add_Item((ITEM_TYPE)m_tUIDesc.iTextureIdx, m_iCooked_CurNum);
	CItem_Manager::Get_Instance()->Use_Item(eIngrd1, iNeedNum1 * m_iCooked_CurNum);
	CItem_Manager::Get_Instance()->Use_Item(eIngrd2, iNeedNum2 * m_iCooked_CurNum);
}

HRESULT CUI_Cook_Food::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_Food::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Cook_Food * CUI_Cook_Food::Create()
{
	CUI_Cook_Food*		pInstance = new CUI_Cook_Food();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_Food");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_Food::Clone(void * pArg)
{
	CUI_Cook_Food*		pInstance = new CUI_Cook_Food(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_Food");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_Food::Free()
{
	__super::Free();
}
