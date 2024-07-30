#include "stdafx.h"
#include "..\Public\UI_Menu.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Menu::CUI_Menu()
	: CUI()
{
}

CUI_Menu::CUI_Menu(const CUI_Menu & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Menu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Menu::Initialize(void * pArg)
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

void CUI_Menu::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Menu::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Menu::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Menu::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOGO), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Menu::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Menu * CUI_Menu::Create()
{
	CUI_Menu*		pInstance = new CUI_Menu();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Menu");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Menu::Clone(void * pArg)
{
	CUI_Menu*		pInstance = new CUI_Menu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Menu::Free()
{
	__super::Free();
}
