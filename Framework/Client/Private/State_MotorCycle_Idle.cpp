#include "stdafx.h"
#include "..\Public\State_MotorCycle_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_MotorCycle_Idle::CState_MotorCycle_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_MotorCycle(pManager, eState)
{
}

CState_MotorCycle_Idle::CState_MotorCycle_Idle(const CState_MotorCycle_Idle& rhs)
	: CState_MotorCycle(rhs)
{
}

HRESULT CState_MotorCycle_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_MOTORCYCLE_RUN);
	m_ReserveStates.push_back(STATE_MOTORCYCLE_JUMP_START);

	/*
		STATE_MOTORCYCLE_IDLE,
		STATE_MOTORCYCLE_JUMP_START,
		STATE_MOTORCYCLE_JUMP_LOOP,
		STATE_MOTORCYCLE_JUMP_RUN,
		STATE_MOTORCYCLE_JUMP_END,
		STATE_MOTORCYCLE_RUN,*/

	return S_OK;
}

HRESULT CState_MotorCycle_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_MotorCycle_Idle::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 0.f;

	m_pTargetModel->Play_Animation(1.f);
}

void CState_MotorCycle_Idle::Tick(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	__super::Tick(fTimeDelta);
}

void CState_MotorCycle_Idle::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	__super::Exit();
}

_bool CState_MotorCycle_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		return true;
	}

	return false;
}


CState_MotorCycle_Idle* CState_MotorCycle_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_MotorCycle_Idle* pInstance = new CState_MotorCycle_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_MotorCycle_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_MotorCycle_Idle::Clone(void* pArg)
{
	CState_MotorCycle_Idle* pInstance = new CState_MotorCycle_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_MotorCycle_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MotorCycle_Idle::Free()
{
}
