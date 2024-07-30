#include "stdafx.h"
#include "..\Public\State_MotorCycle_Run.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_MotorCycle_Run::CState_MotorCycle_Run(CState_Manager* pManager, STATE_TYPE eState)
	: CState_MotorCycle(pManager, eState)
{
}

CState_MotorCycle_Run::CState_MotorCycle_Run(const CState_MotorCycle_Run& rhs)
	: CState_MotorCycle(rhs)
{
}

HRESULT CState_MotorCycle_Run::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_MOTORCYCLE_JUMP_START);
	m_ReserveStates.push_back(STATE_MOTORCYCLE_IDLE);

	/*
		STATE_MOTORCYCLE_IDLE,
		STATE_MOTORCYCLE_JUMP_START,
		STATE_MOTORCYCLE_JUMP_LOOP,
		STATE_MOTORCYCLE_JUMP_RUN,
		STATE_MOTORCYCLE_JUMP_END,
		STATE_MOTORCYCLE_RUN,*/

	return S_OK;
}

HRESULT CState_MotorCycle_Run::Initialize(void* pArg)
{
	return S_OK;
}

void CState_MotorCycle_Run::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
}

void CState_MotorCycle_Run::Tick(_float fTimeDelta)
{
	CState_MotorCycle::Tick(fTimeDelta);
}

void CState_MotorCycle_Run::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_MotorCycle_Run::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D')))
	{
		return true;
	}

	return false;
}


CState_MotorCycle_Run* CState_MotorCycle_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_MotorCycle_Run* pInstance = new CState_MotorCycle_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_MotorCycle_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_MotorCycle_Run::Clone(void* pArg)
{
	CState_MotorCycle_Run* pInstance = new CState_MotorCycle_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_MotorCycle_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MotorCycle_Run::Free()
{
}
