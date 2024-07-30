#include "stdafx.h"
#include "..\Public\UI_Menu_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Menu_Back.h"
#include "UI_Item.h"
#include "Item_Manager.h"
#include "Player.h"
#include "Camera_Target_Position.h"

CUI_Menu_Button::CUI_Menu_Button()
	: CUI()
{
}

CUI_Menu_Button::CUI_Menu_Button(const CUI_Menu_Button & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Menu_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Menu_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 15;
	m_iMenuButton = m_tUIDesc.iTextureIdx;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Menu_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	USEGAMEINSTANCE;	
	IMPL_ONCE(m_bOnce,	
	for (int i = 0; i < 2; ++i)
	{
		m_MenuBacklist.push_back((CUI_Menu_Back*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_MENU_BACK), i));
	});

	SetActive_MenuButton(fTimeDelta);
	Interact_MenuButton(fTimeDelta);
}

void CUI_Menu_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Menu_Button::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

void CUI_Menu_Button::SetActive_MenuButton(_float fTimeDelta)
{
	m_bActive = m_MenuBacklist.front()->Get_MenuActive();

	if (m_bActive)
	{
		m_bRender = true;
		m_fActiveTime += fTimeDelta;
		if (0.55f < m_fActiveTime)
		{
			m_fActiveTime = 0.55f;
		}
	}
	else
	{
		m_bRender = false;
		m_fActiveTime -= fTimeDelta;
		if (0.f > m_fActiveTime)
		{
			m_fActiveTime = 0.f;
		}
	}

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, m_fActiveTime, 0.55f));
	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Menu_Button::Interact_MenuButton(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.4f, m_fY - m_fSizeY * 0.4f
		, m_fX + m_fSizeX * 0.4f, m_fY + m_fSizeY * 0.4f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 1.1f, m_tUIDesc.fInitSizeY * 1.1f, 1.f, 1.f);
	_float4 vCurPoint;

	if (PtInRect(&rcSelect, ptMouse) && m_bActive)
	{
		//마우스 대면 사이즈 커졌다가
		m_fAccTime += fTimeDelta;
		if (0.4f < m_fAccTime)
		{
			m_fAccTime = 0.4f;
		}

		if (KEY_DOWN(VK_LBUTTON))
		{
			// 사이즈 커졌다가 작아짐
			switch (m_iMenuButton)
			{
			case MENUB_EXIT:
				for (auto& elem : m_MenuBacklist)
				{
					elem->Set_MenuActive(false);
					if (pPlayer)
					{
						pPlayer->Set_EnableKeyInput(true);
						FIXCURSOR(true);
					}
				}
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;

			case MENUB_INVENTORY:
				Load_UIData("Inven_Window");
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
					&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 16, 5, { 0.f,0.f },this));
				pGameInstance->Disable_TickGroup(TG_UI);
				pGameInstance->Disable_LateTickGroup(LTG_UI);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				pGameInstance->Disable_LateTickGroup(LTG_UI_PLACENAME);
				break;
				
			case MENUB_EQUIP:
				//Load_UIData("Equip_Window");
				Load_UIData("Equip1");
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
					&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 20, 5, { 0.f,0.f }, this));
				//pPlayer->Set_InEquip_Window(true);
				pGameInstance->Disable_TickGroup(TG_UI);
				pGameInstance->Disable_LateTickGroup(LTG_UI);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				pGameInstance->Disable_LateTickGroup(LTG_UI_PLACENAME);
				break;

			case MENUB_ORDER:
				Load_UIData("Order_Window");
				pGameInstance->Disable_TickGroup(TG_UI);
				pGameInstance->Disable_LateTickGroup(LTG_UI);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;

			default:
				break;
			}
		}
	}
	else //마우스 떼면 사이즈 작아짐
	{
		m_fAccTime -= fTimeDelta;
		if (0.f > m_fAccTime)
		{
			m_fAccTime = 0.f;
		}
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.4f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

HRESULT CUI_Menu_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MENU), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Menu_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Menu_Button * CUI_Menu_Button::Create()
{
	CUI_Menu_Button*		pInstance = new CUI_Menu_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Menu_Button");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Menu_Button::Clone(void * pArg)
{
	CUI_Menu_Button*		pInstance = new CUI_Menu_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Menu_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Menu_Button::Free()
{
	__super::Free();
}
