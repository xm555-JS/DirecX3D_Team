#include "stdafx.h"
#include "..\Public\UI_Equip_Detail.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Equip_Detail::CUI_Equip_Detail()
	: CUI_Equip_Window()
{
}

CUI_Equip_Detail::CUI_Equip_Detail(const CUI_Equip_Detail & rhs)
	: CUI_Equip_Window(rhs)
{
}

HRESULT CUI_Equip_Detail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Equip_Detail::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	m_iPreTextureIdx = m_tUIDesc.iTextureIdx;

	SetUp_UIpos();

	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	pPlayer->Set_Change_SlotWeapon(true);
	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Equip_Detail::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fSizeRatioX = 0.f;
	m_fAlphaRatio = 0.f;
	m_fWaitTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 10;
	m_iPreTextureIdx = m_tUIDesc.iTextureIdx;

	SetUp_UIpos();

	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	pPlayer->Set_Change_SlotWeapon(true);
	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

void CUI_Equip_Detail::Tick(_float fTimeDelta)
{	
	m_iPreTextureIdx = m_tUIDesc.iTextureIdx;

	CUI::Tick(fTimeDelta);
	Create_Equip_Window(fTimeDelta);

	if (0.4f == m_fAccTime)
	{
		m_iPassIdx = 1.f;
	}
}

void CUI_Equip_Detail::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);

	m_fWaitTime += fTimeDelta;

	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (m_tUIDesc.iTextureIdx != m_iPreTextureIdx)
	{
		pPlayer->Set_Change_SlotWeapon(true);

		m_fAccTime = 0.f;
		m_fAlphaRatio = 0.f;
	}
	else
	{
		if (0.5f < m_fWaitTime)
		{
			pPlayer->Set_Change_SlotWeapon(false);
		}
	}
}

HRESULT CUI_Equip_Detail::Render()
{
	FAILEDCHECK(CUI::Render());
	_bool bEdgeFade = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bEdgeFade", &bEdgeFade, sizeof(_bool)));

	return S_OK;
}

HRESULT CUI_Equip_Detail::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_EQUIP_DETAIL), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Equip_Detail::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	_bool bEdgeFade = true;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bEdgeFade", &bEdgeFade, sizeof(_bool)));
	return S_OK;
}

CUI_Equip_Detail* CUI_Equip_Detail::Create()
{
	CUI_Equip_Detail* pInstance = new CUI_Equip_Detail();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Equip_Detail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Equip_Detail::Clone(void* pArg)
{
	CUI_Equip_Detail* pInstance = new CUI_Equip_Detail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Equip_Detail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Equip_Detail::Free()
{
	__super::Free();
}
