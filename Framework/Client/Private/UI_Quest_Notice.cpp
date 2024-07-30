#include "stdafx.h"
#include "..\Public\UI_Quest_Notice.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Quest_Notice::CUI_Quest_Notice()
	: CUI()
{
}

CUI_Quest_Notice::CUI_Quest_Notice(const CUI_Quest_Notice & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Quest_Notice::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Quest_Notice::Initialize(void * pArg)
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

void CUI_Quest_Notice::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	m_fWaitTime += fTimeDelta;


	if (2 > m_tUIDesc.iTextureIdx && 0.5f < m_fWaitTime)
	{
		SetUp_Symbol_Anim(fTimeDelta);
	}
	else if(2 <= m_tUIDesc.iTextureIdx)
	{
		SetUp_Text_Anim(fTimeDelta);
	}
}

void CUI_Quest_Notice::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Quest_Notice::Render()
{
	FAILEDCHECK(CUI::Render());
	_bool	bGlow = false;
	_float	fGlowPower = 0.f;

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	return S_OK;
}

void CUI_Quest_Notice::SetUp_Symbol_Anim(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 8.f, m_tUIDesc.fInitSizeY * 8.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeX, m_tUIDesc.fInitSizeY, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
		m_fAlphaRatio = 0.f;
		GAMEINSTANCE->Add_DeadObject(this);
	}
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));

	m_fSizeX = vCurPoint.x;
	m_fSizeY = vCurPoint.y;
}

void CUI_Quest_Notice::SetUp_Text_Anim(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX * 0.5f + g_iWinCX, 0.5f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fAccTime += fTimeDelta;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
	}
	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));

	m_fX = vCurPoint.x;
	m_fAlphaRatio = vCurPoint.y;
}

HRESULT CUI_Quest_Notice::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_QUEST_NOTICE), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Quest_Notice::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	if(2 > m_tUIDesc.iTextureIdx)
	{
		_bool	bGlow = true;
		_float	fGlowPower = 0.5f;

		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Quest_Notice * CUI_Quest_Notice::Create()
{
	CUI_Quest_Notice*		pInstance = new CUI_Quest_Notice();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Quest_Notice");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Quest_Notice::Clone(void * pArg)
{
	CUI_Quest_Notice*		pInstance = new CUI_Quest_Notice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Quest_Notice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Quest_Notice::Free()
{
	__super::Free();
}
