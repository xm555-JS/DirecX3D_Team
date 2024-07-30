#include "stdafx.h"
#include "..\Public\UI_Logo_Back.h"

#include "GameInstance.h"

CUI_Logo_Back::CUI_Logo_Back()
	: CUI()
{
}

CUI_Logo_Back::CUI_Logo_Back(const CUI_Logo_Back & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Logo_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Logo_Back::Initialize(void * pArg)
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

void CUI_Logo_Back::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (m_bFadeOut)
	{
		m_fAlphaRatio -= fTimeDelta * 0.3f;
		if (0.f > m_fAlphaRatio)
		{
			USEGAMEINSTANCE;
			pGameInstance->Add_DeadObject(this);
			if (LEVEL_GAMEPLAY == m_iCurLevel)
			{
				pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACENAME), ID_UINT(UI, UI_PLACENAME),
					&UIDESC(400.f, 270.f, 500.f, 151.1f, 0.f, 0, 200));
			}
		}
	}
}

void CUI_Logo_Back::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Logo_Back::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Logo_Back::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_SCREEN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Logo_Back::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Logo_Back * CUI_Logo_Back::Create()
{
	CUI_Logo_Back*		pInstance = new CUI_Logo_Back();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Logo_Back");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Logo_Back::Clone(void * pArg)
{
	CUI_Logo_Back*		pInstance = new CUI_Logo_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Logo_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo_Back::Free()
{
	__super::Free();
}
