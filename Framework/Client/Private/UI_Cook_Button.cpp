#include "stdafx.h"
#include "..\Public\UI_Cook_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Level_GamePlay.h"
#include "Camera_Target_Position.h"
#include "Npc_Cooker.h"
#include "UI_Cook_Food.h"


CUI_Cook_Button::CUI_Cook_Button()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_Button::CUI_Cook_Button(const CUI_Cook_Button & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_Button::Initialize(void * pArg)
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

HRESULT CUI_Cook_Button::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;

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

void CUI_Cook_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Interact_Button(fTimeDelta);
	if (COOKB_COOKING == m_tUIDesc.iTextureIdx
		|| COOKB_BACKNUM == m_tUIDesc.iTextureIdx
		|| COOKB_FRONTNUM == m_tUIDesc.iTextureIdx)
	{
		Create_CookingButton(fTimeDelta);
	}
}

void CUI_Cook_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_Button::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Cook_Button::Create_CookingButton(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAlphaTime += fTimeDelta;
	if (0.5f < m_fAlphaTime)
	{
		m_fAlphaTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, m_fAlphaTime, 0.5f));
	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Cook_Button::Pooling_Cook_Objects()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjects = nullptr;

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_INGREDIENT));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_FOOD));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_ICON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_BUTTON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_DETAIL));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	CGameObject* pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_COOK_FOODSELECT));
	pGameInstance->Add_PoolingObject(pGameObject);
}

void CUI_Cook_Button::Interact_Button(_float fTimeDelta)
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
			CUI* pUI = nullptr;
			list<CGameObject*>* pGameObjects = nullptr;
			CNpc_Cooker* pCooker = (CNpc_Cooker*)GAMEINSTANCE->Get_Object(m_iCurLevel, ID_UINT(NPC, NPC_COOKER));
			CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
			_bool bCanCook = false;
			switch (m_tUIDesc.iTextureIdx)
			{
			case COOKB_COOKING:

				//  아이템 개수 감소시키기 그리고 요리 개수 증가시키기
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_FOOD));
				if (pGameObjects)
				{
					for (auto& elem : *pGameObjects)
					{
						if (((CUI_Cook_Food*)elem)->Get_Selected() && 0 < ((CUI_Cook_Food*)elem)->Get_CookCurNum())
						{
							// 랜더 안하기
							pGameInstance->Disable_LateTickGroup(LTG_UI_MENU);
							((CUI_Cook_Food*)elem)->Set_StartCooking(true);
							m_iFoodType = (((CUI_Cook_Food*)elem)->Get_UIDesc().iTextureIdx);
							m_iCookFoodNum = ((CUI_Cook_Food*)elem)->Get_CookCurNum();
							bCanCook = true;
							break;
						}
					}

				}
				if (!bCanCook)
				{
					CUI* pUI = nullptr;
					pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
						&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

					pUI->Set_Delfault_FontText(TEXT("필요한 재료가 부족해 요리를 할 수 없습니다."));
				}
				else
				{
					pGameInstance->Add_Event(EVENT_COOK, &m_iFoodType, &m_iCookFoodNum);
				}
				break;

			case COOKB_FRONTNUM:
				// cookfood list 돌면서 selected인애 curNum 증가시키기 만약 요리할수 없는상태면 안올라가게 그리고 MAxNum이어도 안올라가게
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_FOOD));
				if (pGameObjects)
				{
					for (auto& elem : *pGameObjects)
					{
						if (((CUI_Cook_Food*)elem)->Get_Selected())
						{
							if (((CUI_Cook_Food*)elem)->Get_CanCooked()
								&& ((CUI_Cook_Food*)elem)->Get_CookMaxNum() > ((CUI_Cook_Food*)elem)->Get_CookCurNum())
							{
								((CUI_Cook_Food*)elem)->Set_CookCurNum(1);
							}
							break;
						}
					}
				}
				break;
			
			case COOKB_BACKNUM:	
				// 요리할수 있는상태면 1에서 안내려감
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
				pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_FOOD));
				if (pGameObjects)
				{
					for (auto& elem : *pGameObjects)
					{
						if (((CUI_Cook_Food*)elem)->Get_Selected())
						{
							if ( 1 < ((CUI_Cook_Food*)elem)->Get_CookCurNum())
							{
								((CUI_Cook_Food*)elem)->Set_CookCurNum(-1);
							}
							break;
						}
					}
				}
				break;
			
			case COOKB_EXIT:
			//COOK_WINDOW에 있던거 다지우고 카메라 Target카메라로 플레이어 키입력 받을 수 있게
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				pGameInstance->Able_TickGroup(TG_UI);
				pGameInstance->Able_LateTickGroup(LTG_UI);
				pPlayer->Set_EnableKeyInput(true);
				FIXCURSOR(true);
				KEY_UNLOCK('F');
				Pooling_Cook_Objects();
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;
			
			}
		}
	}
}

HRESULT CUI_Cook_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Cook_Button * CUI_Cook_Button::Create()
{
	CUI_Cook_Button*		pInstance = new CUI_Cook_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_Button");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_Button::Clone(void * pArg)
{
	CUI_Cook_Button*		pInstance = new CUI_Cook_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_Button::Free()
{
	__super::Free();
}
