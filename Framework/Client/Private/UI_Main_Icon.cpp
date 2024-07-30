#include "stdafx.h"
#include "..\Public\UI_Main_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Main_Icon::CUI_Main_Icon()
	: CUI()
{
}

CUI_Main_Icon::CUI_Main_Icon(const CUI_Main_Icon & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Main_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Main_Icon::Initialize(void * pArg)
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

void CUI_Main_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
}

void CUI_Main_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Main_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Main_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MAIN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Main_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Main_Icon * CUI_Main_Icon::Create()
{
	CUI_Main_Icon*		pInstance = new CUI_Main_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Main_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Main_Icon::Clone(void * pArg)
{
	CUI_Main_Icon*		pInstance = new CUI_Main_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Main_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Main_Icon::Free()
{
	__super::Free();
}
