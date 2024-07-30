#include "stdafx.h"
#include "..\Public\UI_Equip_Weapon_M.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Equip_Button.h"
//#include "UI_Equip_Detail.h"

CUI_Equip_Weapon_M::CUI_Equip_Weapon_M()
	: CUI_Equip_Window()
{
}

CUI_Equip_Weapon_M::CUI_Equip_Weapon_M(const CUI_Equip_Weapon_M & rhs)
	: CUI_Equip_Window(rhs)
{
}

HRESULT CUI_Equip_Weapon_M::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Weapon_M::Initialize(void * pArg)
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
	pGameInstance->Add_CallbackList(this, EVENT_WEAPON_CHANGE);
	return S_OK;
}

HRESULT CUI_Equip_Weapon_M::Initialize_Pooling(void* pArg)
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
	pGameInstance->Add_CallbackList(this, EVENT_WEAPON_CHANGE);
	return S_OK;
}

void CUI_Equip_Weapon_M::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	USEGAMEINSTANCE;
	_bool bInRect = false;	
	bInRect = Weapon_Select_Anim(fTimeDelta);

	Create_Equip_Window(fTimeDelta);
	SetUp_Slot_Weapon(WPN_SLOT_M);


	CUI_Equip_Button* pButton = (CUI_Equip_Button*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_EQUIP_BUTTON));
	if (bInRect && KEY_DOWN(VK_LBUTTON))
	{
		(pButton->Get_UI_SelectWeapon())->Set_vUIPos({ m_fX, m_fY });
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
		m_bSelected = true;
	}
	if (pButton && m_fX == (pButton->Get_UI_SelectWeapon())->Get_vUIPos().x)
	{
		m_bSelected = true;
		(pButton->Get_UI_WeaponDetail())->Set_iTextureIdx(m_tUIDesc.iTextureIdx);
	}
	else
	{
		m_bSelected = false;
	}
}


void CUI_Equip_Weapon_M::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Equip_Weapon_M::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Equip_Weapon_M::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_WEAPON_CHANGE == tEvent.eEventType)
	{
		if (tEvent.wParam)
		{
			if (false == *(_bool*)tEvent.wParam && m_bSelected)
			{
				Equip_PlayerWeapon(WPN_SLOT_M, *(WEAPON_TYPE*)tEvent.lParam);
			}
		}
	}
}

HRESULT CUI_Equip_Weapon_M::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_EQUIP_WEAPON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Weapon_M::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Equip_Weapon_M* CUI_Equip_Weapon_M::Create()
{
	CUI_Equip_Weapon_M* pInstance = new CUI_Equip_Weapon_M();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Equip_Weapon_M");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equip_Weapon_M::Clone(void* pArg)
{
	CUI_Equip_Weapon_M* pInstance = new CUI_Equip_Weapon_M(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Equip_Weapon_M");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equip_Weapon_M::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_WEAPON_CHANGE);
}
