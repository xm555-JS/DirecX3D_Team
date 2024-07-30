#include "stdafx.h"
#include "..\Public\UI_Main_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Menu_Back.h"
#include "UI_Food_Window.h"
#include "UI_Food_Icon.h"
#include "Item_Manager.h"
#include "Player.h"
#include "Camera_Target_Position.h"


CUI_Main_Button::CUI_Main_Button()
	: CUI()
{
}

CUI_Main_Button::CUI_Main_Button(const CUI_Main_Button & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Main_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Main_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;
	m_iMainButton = m_tUIDesc.iTextureIdx;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Main_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	USEGAMEINSTANCE;
	IMPL_ONCE(m_bOnce,
	for (int i = 0; i < 2; ++i)
	{
		m_MenuBacklist.push_back((CUI_Menu_Back*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_MENU_BACK), i));
	});
	
	if (m_vecFoodIcon.size())
	{
		CItem_Manager::Get_Instance()->SetUp_FoodInven(m_vecFoodIcon);
	}

}

void CUI_Main_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
	Interact_Menu_Button(fTimeDelta);
}

HRESULT CUI_Main_Button::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Main_Button::Interact_Menu_Button(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;


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
			switch (m_iMainButton)
			{
			case MAINB_ESC:
				for (auto& elem : m_MenuBacklist)
				{
					elem->Set_MenuActive(true);
				}
				if (pPlayer)
				{
					pPlayer->Set_EnableKeyInput(false);
					FIXCURSOR(false);
				}
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;

			case MAINB_FOOD:
				if (m_bActive)
				{
					if (m_pFoodWindow)
					{
						pGameInstance->Add_DeadObject(m_pFoodWindow);
					}
					for(auto& elem : m_vecFoodIcon)
					{
						pGameInstance->Add_DeadObject(elem);
					}

					list<CGameObject*>* pFoodNumlist;
					pFoodNumlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_FOOD_NUM));
					for (auto& elem : *pFoodNumlist)
					{
						pGameInstance->Add_DeadObject(elem);
					}

					m_vecFoodIcon.clear();
					m_tUIDesc.fAngle = 0.f;
					m_bActive = false;
					if (pPlayer)
					{
						pPlayer->Set_EnableKeyInput(true);
						FIXCURSOR(true);
					}

					//MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				}
				else if(!m_bActive)
				{
					Create_Food_Window(fTimeDelta);
					m_tUIDesc.fAngle = 180.f; 
					m_bActive = true;
					if (pPlayer)
					{
						pPlayer->Set_EnableKeyInput(false);
						FIXCURSOR(false);
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else
	{
		if (!PtInRect(&rcSelect, ptMouse) && KEY_DOWN(VK_LBUTTON) && m_bActive)
		{
			if (m_pFoodWindow)
			{
				pGameInstance->Add_DeadObject(m_pFoodWindow);
			}
			for (auto& elem : m_vecFoodIcon)
			{
				if (pPlayer)
				{
					pPlayer->Set_EnableKeyInput(true);
					FIXCURSOR(true);
				}
				pGameInstance->Add_DeadObject(elem);
			}
			list<CGameObject*>* pFoodNumlist;
			pFoodNumlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_FOOD_NUM));
			for (auto& elem : *pFoodNumlist)
			{
				pGameInstance->Add_DeadObject(elem);
			}

			m_vecFoodIcon.clear();
			m_tUIDesc.fAngle = 0.f;
			m_bActive = false;			
		}
	}
}

void CUI_Main_Button::Create_Food_Window(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	json Data;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(TEXT("UI/InvenFood")).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDesc UIDesc;
		string strTemp = "";
		iPrototypeID = elem["iPrototypeID"];
		UIDesc.fInitX = elem["fInitX"];
		UIDesc.fInitY = elem["fInitY"];
		UIDesc.fInitSizeX = elem["fInitSizeX"];
		UIDesc.fInitSizeY = elem["fInitSizeY"];
		UIDesc.fAngle = elem["fAngle"];
		UIDesc.iTextureIdx = elem["iTextureIdx"];
		UIDesc.iRenderLayer = elem["iRenderLayer"];
		strTemp = elem["strText"];
		UIDesc.vFontPos.x = elem["vFontPosx"];
		UIDesc.vFontPos.y = elem["vFontPosy"];
		UIDesc.vFontColor.x = elem["vFontColorx"];
		UIDesc.vFontColor.y = elem["vFontColory"];
		UIDesc.vFontColor.z = elem["vFontColorz"];
		UIDesc.vFontColor.w = elem["vFontColorw"];
		UIDesc.fFontSize = elem["fFontSize"];
		UIDesc.bRenderFontText = elem["bRenderFontText"];
		UIDesc.bRenderFontNum = elem["bRenderFontNum"];

		ID_UINT IDPrototype;
		IDPrototype.iID = iPrototypeID;

		CGameObject* pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);

		CUI_Food_Window* pFoodWindow = dynamic_cast<CUI_Food_Window*>(pUI);
		CUI_Food_Icon*   pFoodIcon = dynamic_cast<CUI_Food_Icon*>(pUI);
		if (pFoodWindow)
		{
			m_pFoodWindow = pFoodWindow;
		}
		if (pFoodIcon)
		{
			m_vecFoodIcon.push_back(pFoodIcon);
		}
	}
	//reverse(m_vecFoodIcon.begin(), m_vecFoodIcon.end());
}

HRESULT CUI_Main_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MAIN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Main_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Main_Button * CUI_Main_Button::Create()
{
	CUI_Main_Button*		pInstance = new CUI_Main_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Main_Button");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Main_Button::Clone(void * pArg)
{
	CUI_Main_Button*		pInstance = new CUI_Main_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Main_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Main_Button::Free()
{
	__super::Free();
}
