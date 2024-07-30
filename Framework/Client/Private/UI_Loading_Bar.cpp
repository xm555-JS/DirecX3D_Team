#include "stdafx.h"
#include "..\Public\UI_Loading_Bar.h"

#include "GameInstance.h"
#include "UI_Loading_Bar.h"

CUI_Loading_Bar::CUI_Loading_Bar()
	: CUI()
{
}

CUI_Loading_Bar::CUI_Loading_Bar(const CUI_Loading_Bar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading_Bar::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 3;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Loading_Bar::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	// m_fsizeRatioX 조절해주는 부분
	if (m_fSizeRatioX < 1.f)
	{
		if (m_fSizeRatioX < 0.45f)
		{
			m_fSizeRatioX += fTimeDelta * 0.03f;
		}
		else if (m_fSizeRatioX < 0.88f)
		{
			m_fSizeRatioX += fTimeDelta * 0.08f;
		}
		if (true == m_bFinish_Loading)
		{
			m_fSizeRatioX = 1.f;
		}
	}
}

void CUI_Loading_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Loading_Bar::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Loading_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_BAR), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Loading_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));


	return S_OK;
}

CUI_Loading_Bar * CUI_Loading_Bar::Create()
{
	CUI_Loading_Bar*		pInstance = new CUI_Loading_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Loading_Bar::Clone(void * pArg)
{
	CUI_Loading_Bar*		pInstance = new CUI_Loading_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Bar::Free()
{
	__super::Free();
}
