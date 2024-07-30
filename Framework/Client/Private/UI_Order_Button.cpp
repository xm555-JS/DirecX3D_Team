#include "stdafx.h"
#include "..\Public\UI_Order_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"
#include "Order_Manager.h"
#include "Player.h"
#include "UI_Order_Anim.h"

CUI_Order_Button::CUI_Order_Button()
	: CUI_Order()
{
}

CUI_Order_Button::CUI_Order_Button(const CUI_Order_Button & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;


	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	
	return S_OK;
}

HRESULT CUI_Order_Button::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fMoveTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	
	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	
	return S_OK;
}

void CUI_Order_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Interact_Button(fTimeDelta);

	if (4 == m_tUIDesc.iTextureIdx || 5 == m_tUIDesc.iTextureIdx)
	{
		Create_Change_Pos(fTimeDelta);
	}
}

void CUI_Order_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_Button::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Order_Button::Pooling_Order_Objects()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjects = nullptr;

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_BUTTON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_ICON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}
}

void CUI_Order_Button::Interact_Button(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{	
			CUI_Order_Anim* pUI_Order_Anim = nullptr;
			switch (m_tUIDesc.iTextureIdx)
			{
			case ORDERB_EXIT:
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
					&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 21, 5, { 0.f,0.f }, this));
				pGameInstance->Able_TickGroup(TG_UI);
				pGameInstance->Able_LateTickGroup(LTG_UI);
				Pooling_Order_Objects();
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;

			case ORDERB_ONE:
				Order_Item(1);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				break;

			case ORDERB_TEN:
				Order_Item(10);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				break;
			}
		}
	}
}

void CUI_Order_Button::Order_Item(_uint iOrderNum)
{
	if (iOrderNum <= CItem_Manager::Get_Instance()->Find_ItemNum(ITEM_BLACKCORE))
	{
		USEGAMEINSTANCE;
		pGameInstance->Add_Event(EVENT_QUEST, nullptr, nullptr);

		pGameInstance->Disable_TickGroup(TG_UI_MENU);
		pGameInstance->Disable_LateTickGroup(LTG_UI_MENU);

		CUI_Order_Anim* pUI_Order_Anim = nullptr;
		CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
		_uint iOrderType;

		CItem_Manager::Get_Instance()->Use_Item(ITEM_BLACKCORE, iOrderNum);
		pUI_Order_Anim = (CUI_Order_Anim*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ORDER_ANIM), ID_UINT(UI, UI_ORDER_ANIM),
			&UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 0, 300));

		if (1 == iOrderNum)
		{
			pUI_Order_Anim->Set_Ten(false);
		}
		else
		{
			pUI_Order_Anim->Set_Ten(true);
			COrder_Manager::Get_Instance()->Reset_TenOrderlist();
		}

		for (_uint i = 0; i < iOrderNum; ++i)
		{
			iOrderType = COrder_Manager::Get_Instance()->Order_Item();
			if (1 == iOrderNum)
			{
				COrder_Manager::Get_Instance()->Set_OneOrder((ORDER_TYPE)iOrderType);
			}
			else
			{
				//iOrderType = COrder_Manager::Get_Instance()->Order_Item();
				COrder_Manager::Get_Instance()->Set_TenOrder((ORDER_TYPE)iOrderType);
			}


			if (ORDER_2GUN > iOrderType)
			{
				CItem_Manager::Get_Instance()->Add_Item((ITEM_TYPE)iOrderType, 1);
			}
			else
			{
				switch (iOrderType)
				{
				case ORDER_2GUN:
					pPlayer->SetUp_OwnWeapon(WEAPON_2GUN);
					break;

				case ORDER_SHIELDAXE:
					pPlayer->SetUp_OwnWeapon(WEAPON_SHIELDAXE);
					break;

				case ORDER_CUBE:
					pPlayer->SetUp_OwnWeapon(WEAPON_CUBE);
					break;
				}
			}
		}
	}
	else
	{
		Create_UI_Alert();
	}
}

void CUI_Order_Button::Create_UI_Alert()
{
	CUI* pUI = nullptr;
	pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
		&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 350, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

	pUI->Set_Delfault_FontText(L"아이템이 부족합니다.");
}

HRESULT CUI_Order_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_Button * CUI_Order_Button::Create()
{
	CUI_Order_Button*		pInstance = new CUI_Order_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_Button");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_Button::Clone(void * pArg)
{
	CUI_Order_Button*		pInstance = new CUI_Order_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_Button::Free()
{
	__super::Free();
}
