#include "stdafx.h"
#include "..\Public\UI_Equip_Remove.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "UI_Equip_Button.h"

CUI_Equip_Remove::CUI_Equip_Remove()
	: CUI_Equip_Window()
{
}

CUI_Equip_Remove::CUI_Equip_Remove(const CUI_Equip_Remove & rhs)
	: CUI_Equip_Window(rhs)
{
}

HRESULT CUI_Equip_Remove::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Remove::Initialize(void * pArg)
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
	
	return S_OK;
}

HRESULT CUI_Equip_Remove::Initialize_Pooling(void* pArg)
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

	return S_OK;
}

void CUI_Equip_Remove::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Interact_Button(fTimeDelta);
	Create_Equip_Window(fTimeDelta);
	Weapon_Select_Anim(fTimeDelta);
}

void CUI_Equip_Remove::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Equip_Remove::Render()
{
	FAILEDCHECK(CUI::Render());
	return S_OK;
}

void CUI_Equip_Remove::Interact_Button(_float fTimeDelta)
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
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
			CUI_Equip_Button* pButton = (CUI_Equip_Button*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_BUTTON));
			_float fTempX = (pButton->Get_UI_SelectWeapon())->Get_vUIPos().x;

			CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

			if (600.f > fTempX)
			{
				pPlayer->Set_Slot_Weapon(WPN_SLOT_L, WEAPON_NONE);
			}
			else if (700.f < fTempX)
			{
				pPlayer->Set_Slot_Weapon(WPN_SLOT_R, WEAPON_NONE);
			}
			else
			{
				//pPlayer->Set_Slot_Weapon(WPN_SLOT_M, WEAPON_NONE);
				CUI* pUI = nullptr;
				pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
					&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

				pUI->Set_Delfault_FontText(wstring(L"사용중인 무기는 해제할 수 없습니다."));
			}
		}
	}
}

HRESULT CUI_Equip_Remove::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_EQUIP_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Remove::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Equip_Remove* CUI_Equip_Remove::Create()
{
	CUI_Equip_Remove* pInstance = new CUI_Equip_Remove();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Equip_Remove");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equip_Remove::Clone(void* pArg)
{
	CUI_Equip_Remove* pInstance = new CUI_Equip_Remove(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Equip_Remove");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equip_Remove::Free()
{
	__super::Free();
}
