#include "stdafx.h"
#include "..\Public\State_Player_Mount_TurnL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Mount_TurnL::CState_Player_Mount_TurnL(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_TurnL::CState_Player_Mount_TurnL(const CState_Player_Mount_TurnL& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_TurnL::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);						//153
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);						//153

	return S_OK;
}

HRESULT CState_Player_Mount_TurnL::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_TurnL::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeBegin.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f);
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_bEndCurState = true;
}

void CState_Player_Mount_TurnL::Tick(_float fTimeDelta)
{
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIng.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f), 0.008f, fTimeDelta);
	Set_PlayerLook(fTimeDelta);

	m_pState_Handler->Set_JumpState(false);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_TurnL::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Mount_TurnL::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	//키 조정해야함
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		return true;
	}

	return false;
}


CState_Player_Mount_TurnL* CState_Player_Mount_TurnL::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_TurnL* pInstance = new CState_Player_Mount_TurnL(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_TurnL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_TurnL::Clone(void* pArg)
{
	CState_Player_Mount_TurnL* pInstance = new CState_Player_Mount_TurnL(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_TurnL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_TurnL::Free()
{
}
