#include "stdafx.h"
#include "..\Public\UI_Attack_Target.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Attack_Target::CUI_Attack_Target()
	: CUI()
{
}

CUI_Attack_Target::CUI_Attack_Target(const CUI_Attack_Target & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Attack_Target::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Attack_Target::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;
	m_tUIDesc.iRenderLayer = 200;
	m_tUIDesc.fInitSizeY = m_tUIDesc.fInitSizeX = 55.f;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Attack_Target::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	
}

void CUI_Attack_Target::LateTick(_float fTimeDelta)
{
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (!pPlayer)
		return;

	CGameObject* pTarget = pPlayer->Get_Target();
	if (!pTarget)
	{
		m_fAlphaRatio = 0.f;
		m_bGlow = false;
	}
	else
	{
		_vector vTargetPos = pTarget->Get_Pos();
		_vector vTargetUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		vTargetPos += vTargetUp * 0.5f;

		_float4 vFinalPos;
		XMStoreFloat4(&vFinalPos, vTargetPos);
		SetUp_ViewportPos(vFinalPos);
		m_fAlphaRatio = 1.f;
		m_bGlow = true;
	}
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Attack_Target::Render()
{
	FAILEDCHECK(CUI::Render());
	_bool	bGlow = false;
	_float	fGlowPower = 0.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	return S_OK;
}

HRESULT CUI_Attack_Target::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ATTACK_TARGET), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Attack_Target::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	_float	fGlowPower = 0.5f;

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &m_bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Attack_Target * CUI_Attack_Target::Create()
{
	CUI_Attack_Target*		pInstance = new CUI_Attack_Target();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Attack_Target");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Attack_Target::Clone(void * pArg)
{
	CUI_Attack_Target*		pInstance = new CUI_Attack_Target(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Attack_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Attack_Target::Free()
{
	__super::Free();
}
