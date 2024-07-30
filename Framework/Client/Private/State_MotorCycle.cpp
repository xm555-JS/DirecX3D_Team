#include "stdafx.h"
#include "..\Public\State_MotorCycle.h"
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

CState_MotorCycle::CState_MotorCycle(CState_Manager* pState_Manager, STATE_TYPE eState)
	: CState(pState_Manager, eState)
{
}

CState_MotorCycle::CState_MotorCycle(const CState_MotorCycle& rhs)
	: CState(rhs)
{
}

void CState_MotorCycle::Enter(void* pArg)
{
}

void CState_MotorCycle::Tick(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	//Set_PlayerLook(fTimeDelta);

	STATE_TYPE	eNextState = CState::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_MotorCycle::Exit(void* pArg)
{
}

void CState_MotorCycle::Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName)
{
}

void CState_MotorCycle::Free()
{
	__super::Free();
}
