#include "stdafx.h"
#include "..\Public\UI_Crystal_Hp_Bar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Unit.h"

CUI_Crystal_Hp_Bar::CUI_Crystal_Hp_Bar()
	: CUI()
{
}

CUI_Crystal_Hp_Bar::CUI_Crystal_Hp_Bar(const CUI_Crystal_Hp_Bar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Crystal_Hp_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Crystal_Hp_Bar::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	switch (m_tUIDesc.iTextureIdx)
	{
	case HPTYPE_BACK:
		m_iPassIdx = 0;
		break;

	case HPTYPE_FRONT:
		m_iPassIdx = 3;
		break;

	case HPTYPE_LERP:
		m_iPassIdx = 3;
		break;

	case HPTYPE_NAME:
		m_iPassIdx = 0;
		break;
	}


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_HPCHANGE);
	return S_OK;
}

void CUI_Crystal_Hp_Bar::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_HpGauge(fTimeDelta);

	if (m_tUIDesc.pGameObject)
	{
		if (m_tUIDesc.pGameObject->Is_Dead())
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}	
}

void CUI_Crystal_Hp_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Crystal_Hp_Bar::Render()
{
	FAILEDCHECK(CUI::Render());
	
	return S_OK;
}

void CUI_Crystal_Hp_Bar::On_EventMessege(EVENTDESC tEvent)
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

void CUI_Crystal_Hp_Bar::SetUp_HpGauge(_float fTimeDelta)
{
	/*if (m_tUIDesc.pGameObject)
	{
		_float fHpRatio = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP / ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;		
	}*/
	if (m_tUIDesc.pGameObject)
	{
		_float fMaxHp = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitInfo()->fHP;
		_float fCurHp = ((CUnit*)m_tUIDesc.pGameObject)->Get_UnitCurInfo()->fHP;
		_vector vStartPoint;
		_vector vTargetPoint;
		_float4 vCurPoint;

		switch (m_tUIDesc.iTextureIdx)
		{
		case HPTYPE_BACK:
			break;

		case HPTYPE_FRONT:
			m_fSizeRatioX = fCurHp / fMaxHp;
			break;

		case HPTYPE_LERP:
			m_fAccTime += fTimeDelta;
			if (0.5f < m_fAccTime)
			{
				m_fAccTime = 0.5f;
			}
			vStartPoint = XMVectorSet(m_fPreHp, 1.f, 1.f, 1.f);
			vTargetPoint = XMVectorSet(fCurHp, 1.f, 1.f, 1.f);

			XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
			m_fSizeRatioX = vCurPoint.x / fMaxHp;
			break;

		default:
			break;
		}
	}
}

HRESULT CUI_Crystal_Hp_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_CRYSTAL_BLOOD), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Crystal_Hp_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));

	return S_OK;
}

CUI_Crystal_Hp_Bar * CUI_Crystal_Hp_Bar::Create()
{
	CUI_Crystal_Hp_Bar*		pInstance = new CUI_Crystal_Hp_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Crystal_Hp_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Crystal_Hp_Bar::Clone(void * pArg)
{
	CUI_Crystal_Hp_Bar*		pInstance = new CUI_Crystal_Hp_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Crystal_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Crystal_Hp_Bar::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_HPCHANGE);
}
