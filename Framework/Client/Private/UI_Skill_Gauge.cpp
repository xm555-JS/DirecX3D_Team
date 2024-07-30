#include "stdafx.h"
#include "..\Public\UI_Skill_Gauge.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Skill_Gauge::CUI_Skill_Gauge()
	: CUI()
{
}

CUI_Skill_Gauge::CUI_Skill_Gauge(const CUI_Skill_Gauge & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skill_Gauge::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 16;

	m_pPlayer = (CPlayer*)PLAYERPTR;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Skill_Gauge::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);

	USEGAMEINSTANCE;

	if (m_pPlayer && m_pPlayer->Get_ResetSkillGauge())
	{
		m_fAccTime += fTimeDelta;

		if (0.5f < m_fAccTime)
		{
			m_fAccTime = 0.f;
			m_pPlayer->Set_ResetSkillGauge(false);
		}
		_vector vStartPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->fSkillGauge, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
		m_fGauge = vCurPoint.x;
	}
	else
	{
		SetUp_SkillGauge();
		m_fAccTime = 0.f;
	}
}

void CUI_Skill_Gauge::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Skill_Gauge::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Skill_Gauge::SetUp_SkillGauge()
{
	if (m_pPlayer)
	{
		m_fGauge = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->fSkillGauge;
	}
}

HRESULT CUI_Skill_Gauge::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_SKILL), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Skill_Gauge::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	//_float2 vSizeRatio = { m_fGauge, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fProgressRatio", &m_fGauge, sizeof(_float)));

	return S_OK;
}

CUI_Skill_Gauge * CUI_Skill_Gauge::Create()
{
	CUI_Skill_Gauge*		pInstance = new CUI_Skill_Gauge();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Skill_Gauge");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Skill_Gauge::Clone(void * pArg)
{
	CUI_Skill_Gauge*		pInstance = new CUI_Skill_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Skill_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Gauge::Free()
{
	__super::Free();
}
