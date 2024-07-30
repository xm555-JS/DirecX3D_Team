#include "stdafx.h"
#include "..\Public\UI_Boss_Hp_Lerp.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Boss_Hp_Lerp::CUI_Boss_Hp_Lerp()
	: CUI()
{
}

CUI_Boss_Hp_Lerp::CUI_Boss_Hp_Lerp(const CUI_Boss_Hp_Lerp & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_Hp_Lerp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_Hp_Lerp::Initialize(void * pArg)
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
	pGameInstance->Add_CallbackList(this, EVENT_HPCHANGE);
	return S_OK;
}

void CUI_Boss_Hp_Lerp::Tick(_float fTimeDelta)
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

void CUI_Boss_Hp_Lerp::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Boss_Hp_Lerp::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

void CUI_Boss_Hp_Lerp::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_HPCHANGE == tEvent.eEventType)
	{
		if (m_tUIDesc.pGameObject == (CUnit*)tEvent.lParam)
		{
			m_fAccTime = 0.f;
			m_fPreHp = *(_float*)(tEvent.wParam);
		}
	}
}

void CUI_Boss_Hp_Lerp::SetUp_HpGauge(_float fTimeDelta)
{
	if (m_tUIDesc.pGameObject)
	{
		//_float fHpRatio = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP / ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;
		
		_float fMaxHp = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;
		_float fCurHp = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP;

		m_fAccTime += fTimeDelta;
		if (0.5f < m_fAccTime)
		{
			m_fAccTime = 0.5f;
		}

		_vector vStartPoint = XMVectorSet(m_fPreHp, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(fCurHp, 1.f, 1.f, 1.f);
		_float4 vCurPoint;
		XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
		_float fHpRatio = vCurPoint.x / fMaxHp;
		
		if (0.2f > fHpRatio)
		{
			m_fSizeRatioX = fHpRatio * 5.f;
		}
		else if (0.4f > fHpRatio)
		{
			m_fSizeRatioX = fHpRatio * 5.f - 1.f;
		}
		else if (0.6f > fHpRatio)
		{
			m_fSizeRatioX = fHpRatio * 5.f - 2.f;
		}
		else if (0.8f > fHpRatio)
		{
			m_fSizeRatioX = fHpRatio * 5.f - 3.f;
		}
		else if (1.f > fHpRatio)
		{
			m_fSizeRatioX = fHpRatio * 5.f - 4.f;
		}

	}
}

HRESULT CUI_Boss_Hp_Lerp::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_BOSS_BLOOD), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Boss_Hp_Lerp::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));

	return S_OK;
}

CUI_Boss_Hp_Lerp * CUI_Boss_Hp_Lerp::Create()
{
	CUI_Boss_Hp_Lerp*		pInstance = new CUI_Boss_Hp_Lerp();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_Hp_Lerp");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Boss_Hp_Lerp::Clone(void * pArg)
{
	CUI_Boss_Hp_Lerp*		pInstance = new CUI_Boss_Hp_Lerp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_Hp_Lerp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_Hp_Lerp::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_HPCHANGE);
}
