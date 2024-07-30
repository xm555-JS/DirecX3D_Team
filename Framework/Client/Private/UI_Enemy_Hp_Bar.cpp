#include "stdafx.h"
#include "..\Public\UI_Enemy_Hp_Bar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Enemy.h"

#include "Imgui_Manager.h"

CUI_Enemy_Hp_Bar::CUI_Enemy_Hp_Bar()
	: CUI_Solid()
{
}

CUI_Enemy_Hp_Bar::CUI_Enemy_Hp_Bar(const CUI_Enemy_Hp_Bar & rhs)
	: CUI_Solid(rhs)
{
}

HRESULT CUI_Enemy_Hp_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Enemy_Hp_Bar::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		memcpy(&m_tUISolidDesc, pArg, sizeof(UISolidDesc));
	}

	switch (m_tUISolidDesc.iTextureIdx)
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
	}

	if (3 > m_tUISolidDesc.iTextureIdx)
	{
		m_vMaxScale = { 2.5f, 0.1375f, 1.f };
		m_vMinScale = { 1.0f, 0.055f, 1.f };
	}
	else
	{
		m_vMaxScale = { 5.f, 0.275f, 1.f };
		m_vMinScale = { 2.5f, 0.1375f, 1.f };
	}

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_HPCHANGE);
	return S_OK;
}

void CUI_Enemy_Hp_Bar::Tick(_float fTimeDelta)
{	
#ifndef _MAPTOOL
	SetUp_Pos();
	SetUp_HpGauge(fTimeDelta);
	Billboard();

	if (m_tUISolidDesc.pGameObject)
	{
		if (((CUnit*)m_tUISolidDesc.pGameObject)->Get_UnitCurInfo()->fHP <= 0)
		{
			GAMEINSTANCE->Add_DeadObject(this);
		}
	}
#endif
}

void CUI_Enemy_Hp_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UISOLID, this);
}

HRESULT CUI_Enemy_Hp_Bar::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI_Solid::Render());
		_bool	bGlow = false;
		_float	fGlowPower = 0.f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}
	return S_OK;
}

void CUI_Enemy_Hp_Bar::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_HPCHANGE == tEvent.eEventType)
	{
		if (m_tUISolidDesc.pGameObject == (CUnit*)tEvent.lParam)
		{
			m_fAccTime = 0.f;
			m_fPreHp = *(_float*)(tEvent.wParam);
		}
	}
}

void CUI_Enemy_Hp_Bar::SetUp_Pos()
{
	if (m_tUISolidDesc.pGameObject)
	{
		if (3 > m_tUISolidDesc.iTextureIdx)
		{
			USEGAMEINSTANCE;
			CTransform* pTransform = m_tUISolidDesc.pGameObject->Get_Transform();
			_float3 vScale = pTransform->Get_Scaled();
			_float4	vPos;
			XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_TRANSLATION));
			vPos.y += m_tUISolidDesc.vDelta.y;
			//vPos.y += DEBUGFLOAT4(0).x;

			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));

			_vector vCamPos = XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition());

			_float fMaxDist = 15.f;
			m_fCurDist = (XMVectorGetX(XMVector3Length(vCamPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION))));

			if (fMaxDist < m_fCurDist)
			{
				m_bRender = false;
			}
			else
			{
				m_bRender = true;
			}

			m_vCurScale.x = (m_fCurDist / fMaxDist * 0.8f + 0.2f) * m_vMaxScale.x;
			m_vCurScale.y = (m_fCurDist / fMaxDist * 0.8f + 0.2f) * m_vMaxScale.y;

			if (m_vMaxScale.x < m_vCurScale.x)
			{
				m_vCurScale.x = m_vMaxScale.x;
				m_vCurScale.y = m_vMaxScale.y;
			}
			m_pTransformCom->Set_Scaled(m_vCurScale);

			m_bRender = ((CEnemy*)m_tUISolidDesc.pGameObject)->Get_IsAttacked();
		}
		else
		{
			USEGAMEINSTANCE;
			CTransform* pTransform = m_tUISolidDesc.pGameObject->Get_Transform();
			_float3 vScale = pTransform->Get_Scaled();
			_float4	vPos;
			XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_TRANSLATION));
			vPos.y += m_tUISolidDesc.vDelta.y;
			//vPos.y += DEBUGFLOAT4(0).x;

			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));

			_vector vCamPos = XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition());

			_float fMaxDist = 15.f;
			m_fCurDist = (XMVectorGetX(XMVector3Length(vCamPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION))));


			m_vCurScale.x = (m_fCurDist / fMaxDist * 0.8f + 0.2f) * m_vMaxScale.x;
			m_vCurScale.y = (m_fCurDist / fMaxDist * 0.8f + 0.2f) * m_vMaxScale.y;

			if (m_vMaxScale.x < m_vCurScale.x)
			{
				m_vCurScale.x = m_vMaxScale.x;
				m_vCurScale.y = m_vMaxScale.y;
			}
			m_pTransformCom->Set_Scaled(m_vCurScale);

			//m_bRender = ((CEnemy*)m_tUISolidDesc.pGameObject)->Get_IsAttacked();
		}
	}
}

void CUI_Enemy_Hp_Bar::SetUp_HpGauge(_float fTimeDelta)
{
	if (m_tUISolidDesc.pGameObject)
	{
		_float fMaxHp = ((CUnit*)m_tUISolidDesc.pGameObject)->Get_UnitInfo()->fHP;
		_float fCurHp = ((CUnit*)m_tUISolidDesc.pGameObject)->Get_UnitCurInfo()->fHP;
		_vector vStartPoint;
		_vector vTargetPoint;
		_float4 vCurPoint;

		switch (m_tUISolidDesc.iTextureIdx)
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

		case HPTYPE_BACK_CRYSTAL:
			break;

		case HPTYPE_FRONT_CRYSTAL:
			m_fSizeRatioX = fCurHp / fMaxHp;
			break;

		case HPTYPE_LERP_CRYSTAL:
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
		}
	}
}

HRESULT CUI_Enemy_Hp_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ENEMY_BLOOD), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Enemy_Hp_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI_Solid::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));

	if (HPTYPE_FRONT == m_tUISolidDesc.iTextureIdx || HPTYPE_FRONT_CRYSTAL == m_tUISolidDesc.iTextureIdx)
	{
		_bool	bGlow = true;
		_float	fGlowPower = 0.5f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fGlowPower", &fGlowPower, sizeof(_float)));
	}
	return S_OK;
}

CUI_Enemy_Hp_Bar * CUI_Enemy_Hp_Bar::Create()
{
	CUI_Enemy_Hp_Bar*		pInstance = new CUI_Enemy_Hp_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Enemy_Hp_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Enemy_Hp_Bar::Clone(void * pArg)
{
	CUI_Enemy_Hp_Bar*		pInstance = new CUI_Enemy_Hp_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Enemy_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Enemy_Hp_Bar::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_HPCHANGE);
}
