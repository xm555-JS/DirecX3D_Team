#include "stdafx.h"
#include "..\Public\UI_Equip_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Equip_Icon::CUI_Equip_Icon()
	: CUI_Equip_Window()
{
}

CUI_Equip_Icon::CUI_Equip_Icon(const CUI_Equip_Icon & rhs)
	: CUI_Equip_Window(rhs)
{
}

HRESULT CUI_Equip_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Icon::Initialize(void * pArg)
{
	// Texture Idx 1 -> Select
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

HRESULT CUI_Equip_Icon::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	m_eSelected_Weapon = WEAPON_END;
	m_eSelected_Slot = WPN_SLOT_END;

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

void CUI_Equip_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Create_Equip_Window(fTimeDelta);
	if (1 == m_tUIDesc.iTextureIdx)
	{
		Select_Icon_Anim(fTimeDelta);
		//cout << m_eSelected_Weapon << endl;
	}
	if (14 == m_tUIDesc.iTextureIdx)
	{
		Weapon_Select_Anim(fTimeDelta);
	}
}

void CUI_Equip_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
	SetUp_Selected_Weapon();
}

HRESULT CUI_Equip_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Equip_Icon::Select_Icon_Anim(_float fTimeDelta)
{
	m_fSizeTime += fTimeDelta * m_fDir;

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX + 20.f, m_tUIDesc.fInitSizeY + 20.f, 1.f, 1.f);

	_float4 vCurPoint;

	if (0.5f < m_fSizeTime)
	{
		m_fAccTime = 0.5f;
		m_fDir *= -1.f;
	}
	if (0.f > m_fSizeTime)
	{
		m_fSizeTime = 0.f;
		m_fDir *= -1.f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fSizeTime, 0.5f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

void CUI_Equip_Icon::SetUp_Selected_Weapon()
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (370.f == m_fX)
	{
		m_eSelected_Slot = WPN_SLOT_L;
		m_eSelected_Weapon = (WEAPON_TYPE)pPlayer->Get_Slot_Weapon(WPN_SLOT_L);
	}
	else if (640.f == m_fX)
	{
		m_eSelected_Slot = WPN_SLOT_M;
		m_eSelected_Weapon = (WEAPON_TYPE)pPlayer->Get_Slot_Weapon(WPN_SLOT_M);
	}
	else if (910.f == m_fX)
	{
		m_eSelected_Slot = WPN_SLOT_R;
		m_eSelected_Weapon = (WEAPON_TYPE)pPlayer->Get_Slot_Weapon(WPN_SLOT_R);
	}
}

HRESULT CUI_Equip_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_EQUIP_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Equip_Icon* CUI_Equip_Icon::Create()
{
	CUI_Equip_Icon* pInstance = new CUI_Equip_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Equip_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equip_Icon::Clone(void* pArg)
{
	CUI_Equip_Icon* pInstance = new CUI_Equip_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Equip_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equip_Icon::Free()
{
	__super::Free();
}
