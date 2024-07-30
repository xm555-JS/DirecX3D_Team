#include "stdafx.h"
#include "..\Public\UI_Dash_Bar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Dash_Bar::CUI_Dash_Bar()
	: CUI()
{
}

CUI_Dash_Bar::CUI_Dash_Bar(const CUI_Dash_Bar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Dash_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dash_Bar::Initialize(void * pArg)
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
	m_pPlayer = (CPlayer*)PLAYERPTR;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Dash_Bar::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);
	if (m_pPlayer)
	{
		//CPlayer::PLAYERDESC* pPlayerDesc = (CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitInfo());
		//CPlayer::PLAYERDESC* pPlayerCurDesc = (CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo());
		_float fMaxDash = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitInfo()))->fDashGauge;
		_float fCurDash = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->fDashGauge;
		m_fSizeRatioX = fCurDash / fMaxDash;
	}
}

void CUI_Dash_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Dash_Bar::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Dash_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MAIN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Dash_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));


	return S_OK;
}

CUI_Dash_Bar * CUI_Dash_Bar::Create()
{
	CUI_Dash_Bar*		pInstance = new CUI_Dash_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Dash_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Dash_Bar::Clone(void * pArg)
{
	CUI_Dash_Bar*		pInstance = new CUI_Dash_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Dash_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dash_Bar::Free()
{
	__super::Free();
}
