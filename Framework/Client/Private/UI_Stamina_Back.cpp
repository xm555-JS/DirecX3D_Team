#include "stdafx.h"
#include "..\Public\UI_Stamina_Back.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Navigation.h"
#include "State_Handler.h"

CUI_Stamina_Back::CUI_Stamina_Back()
	: CUI()
{
}

CUI_Stamina_Back::CUI_Stamina_Back(const CUI_Stamina_Back & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Stamina_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Stamina_Back::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	if (3 == m_tUIDesc.iTextureIdx)
		m_iPassIdx = 8;
	else
		m_iPassIdx = 1;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_STAMINACHANGE);

	m_pPlayer = (CPlayer*)PLAYERPTR;

	return S_OK;
}

void CUI_Stamina_Back::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_ViewPortPos();
	if (3 == m_tUIDesc.iTextureIdx)
	{
		SetUp_StaminaGauge(fTimeDelta);
	}
	Show_StaminaGauge(fTimeDelta);
}

void CUI_Stamina_Back::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Stamina_Back::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Stamina_Back::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_STAMINACHANGE == tEvent.eEventType)
	{
		m_fAccTime = 0.f;
		m_fPreStamina = *(_float*)(tEvent.lParam);
		_float fCurStamina = *(_float*)(tEvent.wParam);
		if (m_fPreStamina > fCurStamina)//감소
		{
			m_bDcrsStamina = true;
			m_bIncrsStamina = false;
		}
		else if (m_fPreStamina < fCurStamina)//증가
		{
			m_bDcrsStamina = false;
			m_bIncrsStamina = true;
		}
	}
}

void CUI_Stamina_Back::SetUp_ViewPortPos()
{
	if (m_pPlayer)
	{
		USEGAMEINSTANCE;
		_vector vPlayerPos = m_pPlayer->Get_Pos();

		_matrix	ViewMatrix = XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW));
		vPlayerPos = XMVector3TransformCoord(vPlayerPos, ViewMatrix);

		_matrix ProjMatrix = XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ));
		vPlayerPos = XMVector3TransformCoord(vPlayerPos, ProjMatrix);

		_float4 vProjPos;
		XMStoreFloat4(&vProjPos, vPlayerPos);

		_float4 vViewportPos;
		{vViewportPos.x = vProjPos.x * (g_iWinCX * 0.5f);
		vViewportPos.y = vProjPos.y * (g_iWinCY * 0.5f);
		vViewportPos.x += g_iWinCX * 0.5f;
		vViewportPos.y = g_iWinCY * 0.5f - vViewportPos.y; }

		m_fX = vViewportPos.x + 100.f;
		m_fY = vViewportPos.y - 150.f;

		if (m_fY < g_iWinCY * 0.5f)
		{
			m_fY = g_iWinCY * 0.5f;
		}
		if (m_fY > 500.f)
		{
			m_fY = 500.f;
		}
	}
}

void CUI_Stamina_Back::SetUp_StaminaGauge(_float fTimeDelta)
{
	if (m_pPlayer)
	{
		_float fMaxStamina = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitInfo()))->fStaminaGauge;
		_float fCurStamina = ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->fStaminaGauge;

		if (m_bIncrsStamina)
		{
			m_fSizeRatioY = fCurStamina / fMaxStamina;
		}
		if (m_bDcrsStamina)
		{
			m_fAccTime += fTimeDelta;
			if (0.5f < m_fAccTime)
			{
				m_fAccTime = 0.5f;
			}
			_vector vStartPoint = XMVectorSet(m_fPreStamina, 1.f, 1.f, 1.f);
			_vector vTargetPoint = XMVectorSet(fCurStamina, 1.f, 1.f, 1.f);
			_float4 vCurPoint;
			XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
			m_fSizeRatioY = vCurPoint.x / fMaxStamina;
		}
	}
}

void CUI_Stamina_Back::Show_StaminaGauge(_float fTimeDelta)
{
	if (m_pPlayer)
	{
		CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(TRANSFORM);
		CNavigation* pPlayerNavi = (CNavigation*)m_pPlayer->Get_Component(NAVIGATION);
		CState_Handler* pPlayerHandler = (CState_Handler*)m_pPlayer->Get_Component(STATE_HANDLER);
		_float			fHightDiff = 0.f;
		pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

		STATE_TYPE eStateType = pPlayerHandler->Get_CurStateType();

		if (1.f < fHightDiff &&

			(STATE_PLAYER_JUMP_A != eStateType &&
			STATE_PLAYER_JUMP_B != eStateType &&
			STATE_PLAYER_JUMP_FALL != eStateType &&
			STATE_PLAYER_ROLL_FORWORD != eStateType &&
			STATE_PLAYER_ROLL_FORWORD_AIR != eStateType &&
			STATE_PLAYER_ROLL_FORWORD_END != eStateType &&
			STATE_PLAYER_ROLL_BACK != eStateType &&
			STATE_PLAYER_ROLL_BACK_AIR != eStateType) ||

			(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT == eStateType ||
			 STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT == eStateType))
		{
			m_fAlphaRatio = 1.f;
			m_fShowTime = 0.f;
		}
		else if (((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitCurInfo()))->fStaminaGauge > ((CPlayer::PLAYERDESC*)(m_pPlayer->Get_UnitInfo()))->fStaminaGauge * 0.9f)
		{
			m_fShowTime += fTimeDelta;
			if (2.f < m_fShowTime)
			{
				m_fAlphaRatio -= fTimeDelta * 1.2f;
				if (0.f > m_fAlphaRatio)
				{
					m_fAlphaRatio = 0.f;
				}
			}
		}
	}
}

HRESULT CUI_Stamina_Back::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PLAYER_STATUS), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Stamina_Back::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { 1.f, (1.f - m_fSizeRatioY) };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bDecrease", &m_bDcrsStamina, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Stamina_Back * CUI_Stamina_Back::Create()
{
	CUI_Stamina_Back*		pInstance = new CUI_Stamina_Back();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Stamina_Back");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Stamina_Back::Clone(void * pArg)
{
	CUI_Stamina_Back*		pInstance = new CUI_Stamina_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Stamina_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Stamina_Back::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_STAMINACHANGE);
}
