#include "stdafx.h"
#include "..\Public\UI_Skill_Enable.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Skill_Enable::CUI_Skill_Enable()
	: CUI()
{
}

CUI_Skill_Enable::CUI_Skill_Enable(const CUI_Skill_Enable & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_Enable::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skill_Enable::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	m_pPlayer = (CPlayer*)PLAYERPTR;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Skill_Enable::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	USEGAMEINSTANCE;

	if (m_pPlayer)
	{
		m_bEnable = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->bAbleSkill_S;
	}


	Resizing(fTimeDelta);
}

void CUI_Skill_Enable::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Skill_Enable::Render()
{
	if (m_bEnable)
	{
		FAILEDCHECK(CUI::Render());
		_bool	bGlow = false;
		_float	fGlowPower = 0.f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}
	return S_OK;
}

void CUI_Skill_Enable::Resizing(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 1.07f, m_tUIDesc.fInitSizeY * 1.07f, 0.4f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.35f < m_fAccTime)
	{
		m_fAccTime = 0.f;
	}
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.35f));
	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
	m_fAlphaRatio = vCurPoint.z;
}

HRESULT CUI_Skill_Enable::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_SKILL), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Skill_Enable::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	
	if (3 == m_tUIDesc.iTextureIdx)
	{
		_bool	bGlow = true;
		_float	fGlowPower = 0.9f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}
	return S_OK;
}

CUI_Skill_Enable * CUI_Skill_Enable::Create()
{
	CUI_Skill_Enable*		pInstance = new CUI_Skill_Enable();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Skill_Enable");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Skill_Enable::Clone(void * pArg)
{
	CUI_Skill_Enable*		pInstance = new CUI_Skill_Enable(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Skill_Enable");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Enable::Free()
{
	__super::Free();
}
