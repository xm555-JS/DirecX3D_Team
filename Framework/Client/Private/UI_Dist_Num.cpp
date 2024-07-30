#include "stdafx.h"
#include "..\Public\UI_Dist_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Dist_Num::CUI_Dist_Num()
	: CUI()
{
}

CUI_Dist_Num::CUI_Dist_Num(const CUI_Dist_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Dist_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dist_Num::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	m_tUIDesc.iRenderLayer = 390;
	SetUp_UIpos();

	USEGAMEINSTANCE;


	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_Dist_Num::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;
	m_bRender = true;

	m_tUIDesc.iRenderLayer = 390;
	SetUp_UIpos();

	USEGAMEINSTANCE;


	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Dist_Num::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Dist_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Dist_Num::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

HRESULT CUI_Dist_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_DIST_NUM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Dist_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Dist_Num * CUI_Dist_Num::Create()
{
	CUI_Dist_Num*		pInstance = new CUI_Dist_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Dist_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Dist_Num::Clone(void * pArg)
{
	CUI_Dist_Num*		pInstance = new CUI_Dist_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Dist_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dist_Num::Free()
{
	__super::Free();
}
