#include "stdafx.h"
#include "..\Public\UI_Alert_Icon.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Alert_Icon::CUI_Alert_Icon()
	: CUI()
{
}

CUI_Alert_Icon::CUI_Alert_Icon(const CUI_Alert_Icon & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Alert_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Alert_Icon::Initialize(void * pArg)
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

	pGameInstance->Add_TickGroup(TG_UI_ALERT, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ALERT, this);
	return S_OK;
}

void CUI_Alert_Icon::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Appear_Alert_Icon(fTimeDelta);
	Disappear_Alert_Icon(fTimeDelta);
}

void CUI_Alert_Icon::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Alert_Icon::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Alert_Icon::Appear_Alert_Icon(_float fTimeDelta)
{
	if (!m_bAppear)
	{
		m_fAppearTime += fTimeDelta;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX + 100.f, 0.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);

		_float4 vCurPoint;
		if (0.2f < m_fAppearTime)
		{
			m_fAppearTime = 0.2f;
			m_bAppear = true;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuinticOut(vStartPoint, vTargetPoint, m_fAppearTime, 0.2f));
		m_fX = vCurPoint.x;
		m_fAlphaRatio = vCurPoint.y;
	}
}

void CUI_Alert_Icon::Disappear_Alert_Icon(_float fTimeDelta)
{
	if (m_bAppear)
	{
		m_fAppearTime += fTimeDelta;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY - 60.f, 0.f, 1.f, 1.f);

		_float4 vCurPoint;
		if (0.5f < m_fDisappearTime)
		{
			m_fDisappearTime = 0.5f;
			GAMEINSTANCE->Add_DeadObject(this);
		}
		if (0.5f < m_fAppearTime)
		{
			m_fDisappearTime += fTimeDelta;

		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fDisappearTime, 0.5f));
		m_fY = vCurPoint.x;
		m_fAlphaRatio = vCurPoint.y;
	}
}

HRESULT CUI_Alert_Icon::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_INGREDIENT), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Alert_Icon::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Alert_Icon * CUI_Alert_Icon::Create()
{
	CUI_Alert_Icon*		pInstance = new CUI_Alert_Icon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Alert_Icon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Alert_Icon::Clone(void * pArg)
{
	CUI_Alert_Icon*		pInstance = new CUI_Alert_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Alert_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Alert_Icon::Free()
{
	__super::Free();
}
