#include "stdafx.h"
#include "..\Public\UI_Boss_Hp_Back.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Boss_Hp_Back::CUI_Boss_Hp_Back()
	: CUI()
{
}

CUI_Boss_Hp_Back::CUI_Boss_Hp_Back(const CUI_Boss_Hp_Back & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_Hp_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_Hp_Back::Initialize(void * pArg)
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

void CUI_Boss_Hp_Back::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_Hp_Texture();
	if (m_tUIDesc.pGameObject)
	{
		if (!((CUnit*)m_tUIDesc.pGameObject)->Get_Target())
		{
			m_bRender = false;
		}
		else
		{
			m_bRender = true;
		}

		if (m_tUIDesc.pGameObject->Is_Dead())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}
}

void CUI_Boss_Hp_Back::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Boss_Hp_Back::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

void CUI_Boss_Hp_Back::SetUp_Hp_Texture()
{
	if (m_tUIDesc.pGameObject)
	{
		_float fHpRatio = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP / ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;
		//m_fSizeRatioX;

		if (0.2f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 4;
		}
		else if (0.4f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 2;
		}
		else if (0.6f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 1;
		}
		else if (0.8f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 0;
		}
		else if (1.f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 3;
		}
	}
}

HRESULT CUI_Boss_Hp_Back::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_BOSS_BLOOD), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Boss_Hp_Back::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Boss_Hp_Back * CUI_Boss_Hp_Back::Create()
{
	CUI_Boss_Hp_Back*		pInstance = new CUI_Boss_Hp_Back();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_Hp_Back");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Boss_Hp_Back::Clone(void * pArg)
{
	CUI_Boss_Hp_Back*		pInstance = new CUI_Boss_Hp_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_Hp_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_Hp_Back::Free()
{
	__super::Free();
}
