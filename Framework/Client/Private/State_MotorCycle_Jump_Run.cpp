#include "stdafx.h"
#include "..\Public\State_MotorCycle_Jump_Run.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_MotorCycle_Jump_Run::CState_MotorCycle_Jump_Run(CState_Manager* pManager, STATE_TYPE eState)
	: CState_MotorCycle(pManager, eState)
{
}

CState_MotorCycle_Jump_Run::CState_MotorCycle_Jump_Run(const CState_MotorCycle_Jump_Run& rhs)
	: CState_MotorCycle(rhs)
{
}

HRESULT CState_MotorCycle_Jump_Run::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_MOTORCYCLE_RUN);
	m_ReserveStates.push_back(STATE_MOTORCYCLE_IDLE);

	return S_OK;
}

HRESULT CState_MotorCycle_Jump_Run::Initialize(void* pArg)
{
	return S_OK;
}

void CState_MotorCycle_Jump_Run::Enter(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 3.f;
}

void CState_MotorCycle_Jump_Run::Tick(_float fTimeDelta)
{
	if (Get_IsFinished())
		m_bEndCurState = true;

	__super::Tick(fTimeDelta);
}

void CState_MotorCycle_Jump_Run::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_MotorCycle_Jump_Run::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float			fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 0.1f)
		return true;

	return false;
}


CState_MotorCycle_Jump_Run* CState_MotorCycle_Jump_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_MotorCycle_Jump_Run* pInstance = new CState_MotorCycle_Jump_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_MotorCycle_Jump_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_MotorCycle_Jump_Run::Clone(void* pArg)
{
	CState_MotorCycle_Jump_Run* pInstance = new CState_MotorCycle_Jump_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_MotorCycle_Jump_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MotorCycle_Jump_Run::Free()
{
}
