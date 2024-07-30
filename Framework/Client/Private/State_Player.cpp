#include "stdafx.h"
#include "..\Public\State_Player.h"
#include "State_Manager.h"
#include "State_Handler.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_Target_Position.h"
#include "Navigation.h"
#include "UI.h"
#include "Channel.h"

#include "Easing_Utillity.h"

CState_Player::CState_Player(CState_Manager* pState_Manager, STATE_TYPE eState)
	: CState(pState_Manager, eState)
{
}

CState_Player::CState_Player(const CState_Player& rhs)
	: CState(rhs)
{
}

void CState_Player::Enter(void* pArg)
{
	m_pPlayer = ((CPlayer*)m_pState_Handler->Get_Player());
	m_pPlayer->Set_NearTarget();
}

void CState_Player::Tick(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	//Set_PlayerLook(fTimeDelta);

	STATE_TYPE	eNextState = CState::Ready_NextState();

	CNavigation* pPlayerNavigation = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(NAVIGATION);
	if (STATE_END != eNextState)
	{
		CTransform* pPlayerTrasform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
		pPlayerTrasform->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root_Bone", &m_RootBoneMatrix);
		m_pTargetModel->Set_ModelEasingRatio(0.2f);
	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root_Bone", &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("Root_Bone");
	XMStoreFloat4x4(&m_RootBoneMatrix,/* pHierarchyNode->Get_OffsetMatrix() **/ pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix())/*pHierarchyNode->Get_TransformMatrix()*/);

	m_pState_Handler->Get_Player()->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix, pPlayerNavigation);
		
		
}										 

void CState_Player::Exit(void* pArg)		 
{
	m_pPlayer->Set_Evasion(false);

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);

	RESET_ONCE(m_bShaked0);
	RESET_ONCE(m_bShaked1);
	RESET_ONCE(m_bShaked2);
	RESET_ONCE(m_bShaked3);
	RESET_ONCE(m_bShaked4);
	RESET_ONCE(m_bShaked5);

	RESET_ONCE(m_bGauge0);
	RESET_ONCE(m_bGauge1);
	RESET_ONCE(m_bGauge2);
	RESET_ONCE(m_bGauge3);
	RESET_ONCE(m_bGauge4);
	RESET_ONCE(m_bGauge5);

	m_bHit0 = false;
	m_bHit1 = false;
	m_bHit2 = false;
	m_bHit3 = false;
	m_bHit4 = false;
	m_bHit5 = false;
}

void CState_Player::Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName)
{
	if (false == *bCheckHit)
	{
		if (iStartKeyFrame <= pTargetAnimation->GetChannel()[0]->Get_NumCurrentKeyFrame() &&
			iEndKeyFrame >= pTargetAnimation->GetChannel()[0]->Get_NumCurrentKeyFrame())

		{
			for (auto& pTargetCol : *pCollider->Get_StayColliders())
			{
				_float4x4 m = *(pTargetCol->Get_Owner()->Get_WorldMatrix());
				_vector v = pTargetCol->Get_Position();
				((CUnit*)pTargetCol->Get_Owner())->Damaged(eElement, fPower);
				if (EffectName != "")
					EFFECT_POS(EffectName, pTargetCol->Get_Owner()->Get_WorldMatrix(), pTargetCol->Get_Position(), XMVectorSet(fRandom(-3.14f, 3.14f), fRandom(-3.14f, 3.14f), fRandom(-3.14f, 3.14f), 0.f));

				*bCheckHit = true;
			}

			for (auto& pTargetCol : *pCollider->Get_EnterColliders())
			{
				((CUnit*)pTargetCol->Get_Owner())->Damaged(eElement, fPower);
				if (EffectName != "")
					EFFECT_POS(EffectName, pTargetCol->Get_Owner()->Get_WorldMatrix(), pTargetCol->Get_Position(), XMVectorSet(fRandom(-3.14f, 3.14f), fRandom(-3.14f, 3.14f), fRandom(-3.14f, 3.14f), 0.f));

				*bCheckHit = true;
			}
		}
	}
}

void CState_Player::Set_PlayerLook(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('W') ||
		KEY_DOWN('A') ||
		KEY_DOWN('S') ||
		KEY_DOWN('D'))
		m_fPlayerLookTimeAcc = 0.f;

	_float	fLerpTime = 0.1f;
	m_fPlayerLookTimeAcc = fTimeDelta;
	CCamera_Target_Position* pCamera = ((CCamera_Target_Position*)pGameInstance->Get_CurrentCamera());
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
	_vector vCurLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	//_vector vCamLook = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D') ||
		nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_Target())
	{
		_vector	vLookDir = { 0.f, 0.f, 0.f, 0.f };

		if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_Target())
		{
			vLookDir += ((CPlayer*)m_pState_Handler->Get_Player())->Get_Target()->Get_Pos()
				- m_pState_Handler->Get_Player()->Get_Pos();
		}

		if (KEY_PRESSING('W'))
			vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		if (KEY_PRESSING('A'))
			vLookDir += -pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

		if (KEY_PRESSING('S'))
			vLookDir += -pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		if (KEY_PRESSING('D'))
			vLookDir += pCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);

		if (0.01f > XMVectorGetX(XMVector3Length(vLookDir)))
			vLookDir = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		m_pState_Handler->Set_PlayerLook(XMVector3Normalize(vLookDir));
	}


	_float	fCurTime = min(1.f, m_fPlayerLookTimeAcc / fLerpTime);

	//vCurLook
	_vector vFinalLook =  XMVectorLerp(vCurLook, XMLoadFloat4(&m_pState_Handler->Get_PlayerLook()), fCurTime);
	
	//_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_pState_Handler->Get_Player()->Get_Transform()->YFixedLook(vFinalLook);
}

void CState_Player::Use_StaminaGauge(_float fGaugeRatio)
{
	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	m_fPreStamina = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fStaminaGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fStaminaGauge -= fGaugeRatio * ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fStaminaGauge;
	GAMEINSTANCE->Add_Event(EVENT_STAMINACHANGE, &m_fPreStamina, &((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fStaminaGauge);
}

_bool CState_Player::Enable_Use_Stamina(CState_Handler* pHandler, _bool bCreateAlertWindow)
{
#if defined(_UI)
	CPlayer* pPlayer = (CPlayer*)pHandler->Get_Player();
	_float fCurStamina = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fStaminaGauge;
	if (0.001f >= fCurStamina)
	{
		if (bCreateAlertWindow)
		{
			CUI* pUI = nullptr;
			pUI = (CUI*)GAMEINSTANCE->Add_GameObject(pHandler->Get_Player()->Get_CurLevel(), ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
				&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

			pUI->Set_Delfault_FontText(TEXT("스태미나가 부족합니다"));
		}
		return false;
	}
	else
	{
		return true;
	}
#endif
	return true;
}

_bool CState_Player::Enable_Use_Dash(CState_Handler* pHandler, _bool bCreateAlertWindow)
{
#if defined(_UI)
	CPlayer* pPlayer = (CPlayer*)pHandler->Get_Player();
	_float fCurDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge;
	if (((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge / 3 > fCurDashGauge)
	{
		if (bCreateAlertWindow)
		{
			CUI* pUI = nullptr;
			pUI = (CUI*)GAMEINSTANCE->Add_GameObject(pHandler->Get_Player()->Get_CurLevel(), ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
				&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

			pUI->Set_Delfault_FontText(TEXT("재사용 대기중 입니다."));
		}
		return false;
	}
	else
	{
		return true;
	}
#endif
	return true;
}

void CState_Player::Free()
{
	__super::Free();
}
