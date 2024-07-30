#include "stdafx.h"
#include "..\Public\State_Player_Mount_Free.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Mount_Free::CState_Player_Mount_Free(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_Free::CState_Player_Mount_Free(const CState_Player_Mount_Free& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_Free::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);

	return S_OK;
}

HRESULT CState_Player_Mount_Free::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_Free::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_pPlayer->Set_Target(nullptr);
}

void CState_Player_Mount_Free::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	m_pState_Handler->Set_JumpState(false);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_Free::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Mount_Free::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D')) &&
		((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Mount_Free* CState_Player_Mount_Free::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_Free* pInstance = new CState_Player_Mount_Free(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_Free::Clone(void* pArg)
{
	CState_Player_Mount_Free* pInstance = new CState_Player_Mount_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_Free::Free()
{
}
