#include "stdafx.h"
#include "..\Public\UI_CurFood_Num.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_CurFood_Num::CUI_CurFood_Num()
	: CUI()
{
}

CUI_CurFood_Num::CUI_CurFood_Num(const CUI_CurFood_Num & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_CurFood_Num::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_CurFood_Num::Initialize(void * pArg)
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

void CUI_CurFood_Num::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_CurFood_Num::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_CurFood_Num::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

HRESULT CUI_CurFood_Num::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ITEM_NUM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_CurFood_Num::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_CurFood_Num * CUI_CurFood_Num::Create()
{
	CUI_CurFood_Num*		pInstance = new CUI_CurFood_Num();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_CurFood_Num");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_CurFood_Num::Clone(void * pArg)
{
	CUI_CurFood_Num*		pInstance = new CUI_CurFood_Num(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_CurFood_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CurFood_Num::Free()
{
	__super::Free();
}
