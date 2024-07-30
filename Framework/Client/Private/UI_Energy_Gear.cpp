#include "stdafx.h"
#include "..\Public\UI_Energy_Gear.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Energy_Gear::CUI_Energy_Gear()
	: CUI()
{
}

CUI_Energy_Gear::CUI_Energy_Gear(const CUI_Energy_Gear & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Energy_Gear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Energy_Gear::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Energy_Gear::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
}

void CUI_Energy_Gear::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Energy_Gear::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Energy_Gear::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ENERGY_GEAR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Energy_Gear::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Energy_Gear * CUI_Energy_Gear::Create()
{
	CUI_Energy_Gear*		pInstance = new CUI_Energy_Gear();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Energy_Gear");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Energy_Gear::Clone(void * pArg)
{
	CUI_Energy_Gear*		pInstance = new CUI_Energy_Gear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Energy_Gear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Energy_Gear::Free()
{
	__super::Free();
}
