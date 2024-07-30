#include "stdafx.h"
#include "..\Public\UI_Cook_Ingredient.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"


CUI_Cook_Ingredient::CUI_Cook_Ingredient()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_Ingredient::CUI_Cook_Ingredient(const CUI_Cook_Ingredient & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_Ingredient::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_Ingredient::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI_MENU, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);
	return S_OK;
}

HRESULT CUI_Cook_Ingredient::Initialize_Pooling(void* pArg)
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

void CUI_Cook_Ingredient::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Cook_Ingredient::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_Ingredient::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Cook_Ingredient::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_Ingredient::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Cook_Ingredient * CUI_Cook_Ingredient::Create()
{
	CUI_Cook_Ingredient*		pInstance = new CUI_Cook_Ingredient();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_Ingredient");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_Ingredient::Clone(void * pArg)
{
	CUI_Cook_Ingredient*		pInstance = new CUI_Cook_Ingredient(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_Ingredient");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_Ingredient::Free()
{
	__super::Free();
}
