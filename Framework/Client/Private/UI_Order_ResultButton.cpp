#include "stdafx.h"
#include "..\Public\UI_Order_ResultButton.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Item_Manager.h"
#include "Order_Manager.h"
#include "Player.h"
#include "UI_Order_Anim.h"
#include "Order.h"

CUI_Order_ResultButton::CUI_Order_ResultButton()
	: CUI_Order()
{
}

CUI_Order_ResultButton::CUI_Order_ResultButton(const CUI_Order_ResultButton & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_ResultButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_ResultButton::Initialize(void * pArg)
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
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);

	return S_OK;
}

HRESULT CUI_Order_ResultButton::Initialize_Pooling(void* pArg)
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
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	
	return S_OK;
}

void CUI_Order_ResultButton::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (RESULTB_SKIP == m_tUIDesc.iTextureIdx)
	{
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY - 300.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);

		_float4 vCurPoint;

		m_fMoveTime += fTimeDelta;
		if (1.5f < m_fMoveTime)
		{
			m_fMoveTime = 1.5f;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, m_fMoveTime, 1.5f));

		m_fY = vCurPoint.x;
	}

	Interact_Button(fTimeDelta);
}

void CUI_Order_ResultButton::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_ResultButton::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Order_ResultButton::Pooling_Order_Objects()
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

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_TENRESULT));
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

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_ORDER_RESULTBUTTON));
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

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(ORDER));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}
}

void CUI_Order_ResultButton::Interact_Button(_float fTimeDelta)
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
			COrder* pOrder = (COrder*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(ORDER));
			switch (m_tUIDesc.iTextureIdx)
			{
			case RESULTB_BACK:
				Pooling_Order_Objects();
				pGameInstance->Able_TickGroup(TG_UI_MENU);
				pGameInstance->Able_LateTickGroup(LTG_UI_MENU);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;

			case RESULTB_ONE:
				Order_Item(1);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				break;

			case RESULTB_TEN:
				Order_Item(10);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				break;

			case RESULTB_SKIP:
				pOrder->Set_Skip();
				pGameInstance->Add_PoolingObject(this);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				break;
			}
		}
	}
}

void CUI_Order_ResultButton::Order_Item(_uint iOrderNum)
{
	if (iOrderNum <= CItem_Manager::Get_Instance()->Find_ItemNum(ITEM_BLACKCORE))
	{
		USEGAMEINSTANCE;
		Pooling_Order_Objects();
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
			if (1 == iOrderNum)
			{
				iOrderType = COrder_Manager::Get_Instance()->Order_Item();
				COrder_Manager::Get_Instance()->Set_OneOrder((ORDER_TYPE)iOrderType);
			}
			else
			{
				iOrderType = COrder_Manager::Get_Instance()->Order_Item();
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

void CUI_Order_ResultButton::Create_UI_Alert()
{
	CUI* pUI = nullptr;
	pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
		&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 350, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

	pUI->Set_Delfault_FontText(L"아이템이 부족합니다.");
}

HRESULT CUI_Order_ResultButton::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_RESULT), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_ResultButton::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_ResultButton * CUI_Order_ResultButton::Create()
{
	CUI_Order_ResultButton*		pInstance = new CUI_Order_ResultButton();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_ResultButton");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_ResultButton::Clone(void * pArg)
{
	CUI_Order_ResultButton*		pInstance = new CUI_Order_ResultButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_ResultButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_ResultButton::Free()
{
	__super::Free();
}
