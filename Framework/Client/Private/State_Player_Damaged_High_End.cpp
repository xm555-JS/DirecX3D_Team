#include "stdafx.h"
#include "..\Public\State_Player_Damaged_High_End.h"
#include "State_Player_Damaged_High_Begin.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"

#include "Player.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Damaged_High_End::CState_Player_Damaged_High_End(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Damaged_High_End::CState_Player_Damaged_High_End(const CState_Player_Damaged_High_End& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Damaged_High_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Damaged_High_End::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Damaged_High_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = false;
	m_fAnimSpeed = 2.f;
}

void CState_Player_Damaged_High_End::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Damaged_High_End::Exit(void* pArg)
{
	__super::Exit();

	((CPlayer*)m_pState_Handler->Get_Player())->Set_Damaged(2, false);
}

_bool CState_Player_Damaged_High_End::Check_Condition(CState_Handler* pHandler)
{

	return true;
}


CState_Player_Damaged_High_End* CState_Player_Damaged_High_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Damaged_High_End* pInstance = new CState_Player_Damaged_High_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Damaged_High_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Damaged_High_End::Clone(void* pArg)
{
	CState_Player_Damaged_High_End* pInstance = new CState_Player_Damaged_High_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Damaged_High_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Damaged_High_End::Free()
{
}
