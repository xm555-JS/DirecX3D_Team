#include "stdafx.h"
#include "..\Public\UI_Boss_Hp_Bar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Boss_Hp_Bar::CUI_Boss_Hp_Bar()
	: CUI()
{
}

CUI_Boss_Hp_Bar::CUI_Boss_Hp_Bar(const CUI_Boss_Hp_Bar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_Hp_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_Hp_Bar::Initialize(void * pArg)
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

void CUI_Boss_Hp_Bar::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_HpGauge(fTimeDelta);

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

void CUI_Boss_Hp_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Boss_Hp_Bar::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

void CUI_Boss_Hp_Bar::SetUp_HpGauge(_float fTimeDelta)
{
	if (m_tUIDesc.pGameObject)
	{
		_float fHpRatio = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP / ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;
		//m_fSizeRatioX;
		
		if (0.2f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 2;
			m_fSizeRatioX = fHpRatio * 5.f;
		}
		else if (0.4f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 1;
			m_fSizeRatioX = fHpRatio * 5.f - 1.f;
		}
		else if (0.6f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 0;
			m_fSizeRatioX = fHpRatio * 5.f - 2.f;
		}
		else if (0.8f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 3;
			m_fSizeRatioX = fHpRatio * 5.f - 3.f;
		}
		else if (1.f >= fHpRatio)
		{
			m_tUIDesc.iTextureIdx = 2;
			m_fSizeRatioX = fHpRatio * 5.f - 4.f;
		}

	}
}

HRESULT CUI_Boss_Hp_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_BOSS_BLOOD), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Boss_Hp_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));

	return S_OK;
}

CUI_Boss_Hp_Bar * CUI_Boss_Hp_Bar::Create()
{
	CUI_Boss_Hp_Bar*		pInstance = new CUI_Boss_Hp_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_Hp_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Boss_Hp_Bar::Clone(void * pArg)
{
	CUI_Boss_Hp_Bar*		pInstance = new CUI_Boss_Hp_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_Hp_Bar::Free()
{
	__super::Free();
}
