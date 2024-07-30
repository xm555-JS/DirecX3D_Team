#include "stdafx.h"
#include "..\Public\UI_Logo.h"

#include "GameInstance.h"
#include "UI_Logo_Back.h"

CUI_Logo::CUI_Logo()
	: CUI()
{
}

CUI_Logo::CUI_Logo(const CUI_Logo & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Logo::Initialize(void * pArg)
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

void CUI_Logo::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	m_fAlphaRatio += m_fDir * 0.65f * fTimeDelta;
	if (1.f < m_fAlphaRatio)
	{
		m_fDir *= -1.f;
	}
	if (0.f > m_fAlphaRatio)
	{
		USEGAMEINSTANCE;
		CUI_Logo_Back* pLogo_Back = (CUI_Logo_Back*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_LOGO_BACK));
		pLogo_Back->Set_FadeOut();
		pGameInstance->Add_DeadObject(this);
	}

}

void CUI_Logo::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Logo::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Logo::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOGO), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Logo::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));


	return S_OK;
}

CUI_Logo * CUI_Logo::Create()
{
	CUI_Logo*		pInstance = new CUI_Logo();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Logo");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Logo::Clone(void * pArg)
{
	CUI_Logo*		pInstance = new CUI_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo::Free()
{
	__super::Free();
}
