#include "stdafx.h"
#include "..\Public\UI_Boss_Name.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Boss_Name::CUI_Boss_Name()
	: CUI()
{
}

CUI_Boss_Name::CUI_Boss_Name(const CUI_Boss_Name & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_Name::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_Name::Initialize(void * pArg)
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

void CUI_Boss_Name::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
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

void CUI_Boss_Name::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Boss_Name::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

HRESULT CUI_Boss_Name::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_BOSS_NAME), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Boss_Name::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Boss_Name * CUI_Boss_Name::Create()
{
	CUI_Boss_Name*		pInstance = new CUI_Boss_Name();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_Name");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Boss_Name::Clone(void * pArg)
{
	CUI_Boss_Name*		pInstance = new CUI_Boss_Name(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_Name");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_Name::Free()
{
	__super::Free();
}
