#include "stdafx.h"
#include "..\Public\State_MotorCycle_Jump_Start.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_MotorCycle_Jump_Start::CState_MotorCycle_Jump_Start(CState_Manager* pManager, STATE_TYPE eState)
	: CState_MotorCycle(pManager, eState)
{
}

CState_MotorCycle_Jump_Start::CState_MotorCycle_Jump_Start(const CState_MotorCycle_Jump_Start& rhs)
	: CState_MotorCycle(rhs)
{
}

HRESULT CState_MotorCycle_Jump_Start::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_MOTORCYCLE_JUMP_LOOP);

	/*
		STATE_MOTORCYCLE_IDLE,
		STATE_MOTORCYCLE_JUMP_START,
		STATE_MOTORCYCLE_JUMP_LOOP,
		STATE_MOTORCYCLE_JUMP_RUN,
		STATE_MOTORCYCLE_JUMP_END,
		STATE_MOTORCYCLE_RUN,*/

	return S_OK;
}

HRESULT CState_MotorCycle_Jump_Start::Initialize(void* pArg)
{
	return S_OK;
}

void CState_MotorCycle_Jump_Start::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 5.f;
}

void CState_MotorCycle_Jump_Start::Tick(_float fTimeDelta)
{
	if (Get_IsFinished())
		m_bEndCurState = true;

	__super::Tick(fTimeDelta);
}

void CState_MotorCycle_Jump_Start::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_MotorCycle_Jump_Start::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE))
	{
		return true;
	}

	return false;
}


CState_MotorCycle_Jump_Start* CState_MotorCycle_Jump_Start::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_MotorCycle_Jump_Start* pInstance = new CState_MotorCycle_Jump_Start(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_MotorCycle_Jump_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_MotorCycle_Jump_Start::Clone(void* pArg)
{
	CState_MotorCycle_Jump_Start* pInstance = new CState_MotorCycle_Jump_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_MotorCycle_Jump_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MotorCycle_Jump_Start::Free()
{
}
