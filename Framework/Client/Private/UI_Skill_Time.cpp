#include "stdafx.h"
#include "..\Public\UI_Skill_Time.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Skill_Time::CUI_Skill_Time()
	: CUI()
{
}

CUI_Skill_Time::CUI_Skill_Time(const CUI_Skill_Time & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_Time::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skill_Time::Initialize(void * pArg)
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

void CUI_Skill_Time::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Skill_Time::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Skill_Time::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Skill_Time::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_INGRD_NUM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Skill_Time::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Skill_Time * CUI_Skill_Time::Create()
{
	CUI_Skill_Time*		pInstance = new CUI_Skill_Time();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Skill_Time");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Skill_Time::Clone(void * pArg)
{
	CUI_Skill_Time*		pInstance = new CUI_Skill_Time(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Skill_Time");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Time::Free()
{
	__super::Free();
}
