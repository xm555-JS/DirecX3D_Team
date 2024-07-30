#include "stdafx.h"
#include "..\Public\State_MotorCycle_Jump_Loop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_MotorCycle_Jump_Loop::CState_MotorCycle_Jump_Loop(CState_Manager* pManager, STATE_TYPE eState)
	: CState_MotorCycle(pManager, eState)
{
}

CState_MotorCycle_Jump_Loop::CState_MotorCycle_Jump_Loop(const CState_MotorCycle_Jump_Loop& rhs)
	: CState_MotorCycle(rhs)
{
}

HRESULT CState_MotorCycle_Jump_Loop::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_MotorCycle_Jump_Loop::Initialize(void* pArg)
{
	m_ReserveStates.push_back(STATE_MOTORCYCLE_JUMP_RUN);

	/*
		STATE_MOTORCYCLE_IDLE,
		STATE_MOTORCYCLE_JUMP_START,
		STATE_MOTORCYCLE_JUMP_LOOP,
		STATE_MOTORCYCLE_JUMP_RUN,
		STATE_MOTORCYCLE_JUMP_END,
		STATE_MOTORCYCLE_RUN,*/
	return S_OK;
}

void CState_MotorCycle_Jump_Loop::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
}

void CState_MotorCycle_Jump_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CState_MotorCycle_Jump_Loop::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_MotorCycle_Jump_Loop::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_MotorCycle_Jump_Loop* CState_MotorCycle_Jump_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_MotorCycle_Jump_Loop* pInstance = new CState_MotorCycle_Jump_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_MotorCycle_Jump_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_MotorCycle_Jump_Loop::Clone(void* pArg)
{
	CState_MotorCycle_Jump_Loop* pInstance = new CState_MotorCycle_Jump_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_MotorCycle_Jump_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MotorCycle_Jump_Loop::Free()
{
}
