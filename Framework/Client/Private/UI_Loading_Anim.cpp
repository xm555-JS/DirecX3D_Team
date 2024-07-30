#include "stdafx.h"
#include "..\Public\UI_Loading_Anim.h"

#include "GameInstance.h"

CUI_Loading_Anim::CUI_Loading_Anim()
	: CUI()
{
}

CUI_Loading_Anim::CUI_Loading_Anim(const CUI_Loading_Anim & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading_Anim::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Loading_Anim::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	if(LEVEL_EDIT_UI!=m_iCurLevel )
		Move_Frame(0.07f, fTimeDelta);
}

void CUI_Loading_Anim::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Loading_Anim::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Loading_Anim::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_ANIM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Loading_Anim::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	_float fAlphaRatio = 0.5f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Loading_Anim * CUI_Loading_Anim::Create()
{
	CUI_Loading_Anim*		pInstance = new CUI_Loading_Anim();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_Anim");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Loading_Anim::Clone(void * pArg)
{
	CUI_Loading_Anim*		pInstance = new CUI_Loading_Anim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Anim::Free()
{
	__super::Free();
}
