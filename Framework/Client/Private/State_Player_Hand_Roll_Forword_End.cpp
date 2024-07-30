#include "stdafx.h"
#include "..\Public\State_Player_Hand_Roll_Forword_End.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"

CState_Player_Hand_Roll_Forword_End::CState_Player_Hand_Roll_Forword_End(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Hand_Roll_Forword_End::CState_Player_Hand_Roll_Forword_End(const CState_Player_Hand_Roll_Forword_End& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Hand_Roll_Forword_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Hand_Roll_Forword_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Hand_Roll_Forword_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.5f;
	m_bEndCurState = false;
	m_fKeyPressTimeAcc = 0.f;
}

void CState_Player_Hand_Roll_Forword_End::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
		KEY_DOWN(VK_SPACE))
	{
		m_bEndCurState = true;
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Hand_Roll_Forword_End::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Hand_Roll_Forword_End::Check_Condition(CState_Handler* pHandler)
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


CState_Player_Hand_Roll_Forword_End* CState_Player_Hand_Roll_Forword_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Hand_Roll_Forword_End* pInstance = new CState_Player_Hand_Roll_Forword_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Hand_Roll_Forword_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Hand_Roll_Forword_End::Clone(void* pArg)
{
	CState_Player_Hand_Roll_Forword_End* pInstance = new CState_Player_Hand_Roll_Forword_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Hand_Roll_Forword_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Hand_Roll_Forword_End::Free()
{
}
