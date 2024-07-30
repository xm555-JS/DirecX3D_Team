#include "stdafx.h"
#include "..\Public\UI_Cook_GaugeNum.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"


CUI_Cook_GaugeNum::CUI_Cook_GaugeNum()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_GaugeNum::CUI_Cook_GaugeNum(const CUI_Cook_GaugeNum & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_GaugeNum::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_GaugeNum::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	return S_OK;
}

HRESULT CUI_Cook_GaugeNum::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	return S_OK;
}

void CUI_Cook_GaugeNum::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

}

void CUI_Cook_GaugeNum::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_GaugeNum::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Cook_GaugeNum::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_PROGRESS), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_GaugeNum::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Cook_GaugeNum * CUI_Cook_GaugeNum::Create()
{
	CUI_Cook_GaugeNum*		pInstance = new CUI_Cook_GaugeNum();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_GaugeNum");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_GaugeNum::Clone(void * pArg)
{
	CUI_Cook_GaugeNum*		pInstance = new CUI_Cook_GaugeNum(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_GaugeNum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_GaugeNum::Free()
{
	__super::Free();
}
