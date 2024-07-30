#include "stdafx.h"
#include "..\Public\UI_Shortkey.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Shortkey::CUI_Shortkey()
	: CUI()
{
}

CUI_Shortkey::CUI_Shortkey(const CUI_Shortkey & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Shortkey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Shortkey::Initialize(void * pArg)
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

void CUI_Shortkey::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
}

void CUI_Shortkey::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Shortkey::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Shortkey::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_SHORTKEY), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Shortkey::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Shortkey * CUI_Shortkey::Create()
{
	CUI_Shortkey*		pInstance = new CUI_Shortkey();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Shortkey");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Shortkey::Clone(void * pArg)
{
	CUI_Shortkey*		pInstance = new CUI_Shortkey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Shortkey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shortkey::Free()
{
	__super::Free();
}
