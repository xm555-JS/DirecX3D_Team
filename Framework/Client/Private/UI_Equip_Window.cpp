#include "stdafx.h"
#include "..\Public\UI_Equip_Window.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"


CUI_Equip_Window::CUI_Equip_Window()
	: CUI()
{
}

CUI_Equip_Window::CUI_Equip_Window(const CUI_Equip_Window & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Equip_Window::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Window::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	return S_OK;
}

HRESULT CUI_Equip_Window::Initialize_Pooling(void* pArg)
{

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Equip_Window::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Equip_Window::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Equip_Window::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Equip_Window::Create_Equip_Window(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.4f < m_fAccTime)
	{
		m_fAccTime = 0.4f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, m_fAccTime, 0.4f));
	m_fSizeRatioX = m_fAlphaRatio = vCurPoint.x;
}

_bool CUI_Equip_Window::Weapon_Select_Anim(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 1.1f, m_tUIDesc.fInitSizeY *1.1f, 1.f, 1.f);
	_float4 vCurPoint;

	_bool bSelect = false;
	if (PtInRect(&rcSelect, ptMouse))
	{
		bSelect = true;
		m_fSizeTime += fTimeDelta;
		if (0.2f < m_fSizeTime)
		{
			m_fSizeTime = 0.2f;
		}
	}
	else
	{
		m_fSizeTime -= fTimeDelta;
		if (0.f > m_fSizeTime)
		{
			m_fSizeTime = 0.f;
		}
	}
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fSizeTime, 0.2f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;

	return bSelect;
}

void CUI_Equip_Window::SetUp_Slot_Weapon(_uint iWeaponSlot)
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (pPlayer)
	{
		_uint iType = pPlayer->Get_Slot_Weapon(iWeaponSlot);
		switch (iType)
		{
		case WEAPON_SWORD:
			m_tUIDesc.iTextureIdx = 2;
			break;

		case WEAPON_2GUN:
			m_tUIDesc.iTextureIdx = 1;
			break;

		case WEAPON_CUBE:
			m_tUIDesc.iTextureIdx = 3;
			break;

		case WEAPON_SHIELDAXE:
			m_tUIDesc.iTextureIdx = 0;
			break;

		case WEAPON_NONE:
			m_tUIDesc.iTextureIdx = 4;
			break;
		}
	}
}

void CUI_Equip_Window::Equip_PlayerWeapon(_uint iWeaponSlot, WEAPON_TYPE eWeaponType)
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (pPlayer)
	{
		pPlayer->Set_Slot_Weapon(iWeaponSlot, eWeaponType);
	}
}

HRESULT CUI_Equip_Window::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOGO), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Window::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

void CUI_Equip_Window::Free()
{
	__super::Free();
}
