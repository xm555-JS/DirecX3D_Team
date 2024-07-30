#include "stdafx.h"
#include "..\Public\UI_Cook_GaugeBar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"


CUI_Cook_GaugeBar::CUI_Cook_GaugeBar()
	: CUI_Cook_WIndow()
{
}

CUI_Cook_GaugeBar::CUI_Cook_GaugeBar(const CUI_Cook_GaugeBar & rhs)
	: CUI_Cook_WIndow(rhs)
{
}

HRESULT CUI_Cook_GaugeBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Cook_GaugeBar::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 16;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	m_iSoundChannel = MGR(CSound_Manager)->PlaySound(TEXT("UI_Cooking.wav"), CSound_Manager::CHANNEL_UI, 0.45f);
	return S_OK;
}

HRESULT CUI_Cook_GaugeBar::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fAccTime = 0.f;
	m_fProgressRatio = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 16;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	m_iSoundChannel = MGR(CSound_Manager)->PlaySound(TEXT("UI_Cooking.wav"), CSound_Manager::CHANNEL_UI, 0.35f);

	return S_OK;
}

void CUI_Cook_GaugeBar::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	m_fProgressRatio += 0.4f * fTimeDelta;

	if (1.1f < m_fProgressRatio)
	{
		GAMEINSTANCE->Add_PoolingObject(this);
		MGR(CSound_Manager)->StopSound((CSound_Manager::CHANNELID)m_iSoundChannel);
		//MGR(CSound_Manager)->PlaySound(TEXT("UI_Cooking_End.wav"), CSound_Manager::CHANNEL_UI, 0.40f);
	}
}

void CUI_Cook_GaugeBar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Cook_GaugeBar::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Cook_GaugeBar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_COOK_PROGRESS), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Cook_GaugeBar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fProgressRatio", &m_fProgressRatio, sizeof(_float)));
	return S_OK;
}

CUI_Cook_GaugeBar * CUI_Cook_GaugeBar::Create()
{
	CUI_Cook_GaugeBar*		pInstance = new CUI_Cook_GaugeBar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Cook_GaugeBar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Cook_GaugeBar::Clone(void * pArg)
{
	CUI_Cook_GaugeBar*		pInstance = new CUI_Cook_GaugeBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Cook_GaugeBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cook_GaugeBar::Free()
{
	__super::Free();
}
