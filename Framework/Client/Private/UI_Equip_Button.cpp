#include "stdafx.h"
#include "..\Public\UI_Equip_Button.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Equip_Button::CUI_Equip_Button()
	: CUI_Equip_Window()
{
}

CUI_Equip_Button::CUI_Equip_Button(const CUI_Equip_Button & rhs)
	: CUI_Equip_Window(rhs)
{
}

HRESULT CUI_Equip_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Button::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);

	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	_uint iDetailTexIdx = 0;
	if (pPlayer)
	{
		_uint iType = pPlayer->Get_Slot_Weapon(WPN_SLOT_L);
		switch (iType)
		{
		case WEAPON_SWORD:
			iDetailTexIdx = 2;
			break;

		case WEAPON_2GUN:
			iDetailTexIdx = 1;
			break;

		case WEAPON_CUBE:
			iDetailTexIdx = 3;
			break;

		case WEAPON_SHIELDAXE:
			iDetailTexIdx = 0;
			break;
		}
	}

	m_pUI_SelectWeapon = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_EQUIP_ICON), ID_UINT(UI, UI_EQUIP_ICON),
		&UIDESC(370.f, 145.f, 240.f, 233.f, 0.f, 1, 900));
	m_pUI_WeaponDetail = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_EQUIP_DETAIL), ID_UINT(UI, UI_EQUIP_DETAIL),
		&UIDESC(1050.f, 470.f, 350.f, 425.f, 0.f, iDetailTexIdx, 900));

	return S_OK;
}

HRESULT CUI_Equip_Button::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);

	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	_uint iDetailTexIdx = 0;
	if (pPlayer)
	{
		_uint iType = pPlayer->Get_Slot_Weapon(WPN_SLOT_L);
		switch (iType)
		{
		case WEAPON_SWORD:
			iDetailTexIdx = 2;
			break;

		case WEAPON_2GUN:
			iDetailTexIdx = 1;
			break;

		case WEAPON_CUBE:
			iDetailTexIdx = 3;
			break;

		case WEAPON_SHIELDAXE:
			iDetailTexIdx = 0;
			break;
		}
	}

	m_pUI_SelectWeapon = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_EQUIP_ICON), ID_UINT(UI, UI_EQUIP_ICON),
		&UIDESC(370.f, 145.f, 225.f, 220.f, 0.f, 1, 900));
	m_pUI_WeaponDetail = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_EQUIP_DETAIL), ID_UINT(UI, UI_EQUIP_DETAIL),
		&UIDESC(1050.f, 470.f, 350.f, 425.f, 0.f, iDetailTexIdx, 900));
	return S_OK;
}

void CUI_Equip_Button::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Interact_Button(fTimeDelta);
	Create_Equip_Window(fTimeDelta);
}

void CUI_Equip_Button::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Equip_Button::Render()
{
	FAILEDCHECK(CUI::Render());
	return S_OK;
}

void CUI_Equip_Button::Interact_Button(_float fTimeDelta)
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
			CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
			switch (m_tUIDesc.iTextureIdx)
			{
			
			case EQUIPB_EXIT:
				Pooling_Equip_Objects();
				pPlayer->Set_InEquip_Window(false);
				pPlayer->SetUp_Cur_HandWeapon();
				pGameInstance->Able_TickGroup(TG_UI);
				pGameInstance->Able_LateTickGroup(LTG_UI);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
				break;
			}
		}
	}
}

void CUI_Equip_Button::Pooling_Equip_Objects()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjects = nullptr;
	CGameObject* pGameObject = nullptr;

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_EQUIP_BUTTON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_EQUIP_ICON));
	if (pGameObjects)
	{
		for (auto& elem : *pGameObjects)
		{
			pGameInstance->Add_PoolingObject(elem);
		}
		pGameObjects->clear();
	}

	pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_WEAPON_L));
	pGameInstance->Add_PoolingObject(pGameObject);

	pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_WEAPON_M));
	pGameInstance->Add_PoolingObject(pGameObject);

	pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_WEAPON_R));
	pGameInstance->Add_PoolingObject(pGameObject);

	pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_DETAIL));
	pGameInstance->Add_PoolingObject(pGameObject);

	pGameObject = pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_REMOVE));
	pGameInstance->Add_PoolingObject(pGameObject);

	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
		&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 20, 5, { 0.f,0.f }, this));
}

HRESULT CUI_Equip_Button::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_EQUIP_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Button::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Equip_Button* CUI_Equip_Button::Create()
{
	CUI_Equip_Button* pInstance = new CUI_Equip_Button();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Equip_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equip_Button::Clone(void* pArg)
{
	CUI_Equip_Button* pInstance = new CUI_Equip_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Equip_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equip_Button::Free()
{
	__super::Free();
}
