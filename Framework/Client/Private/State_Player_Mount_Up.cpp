#include "stdafx.h"
#include "..\Public\State_Player_Mount_Up.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Mount_Up::CState_Player_Mount_Up(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_Up::CState_Player_Mount_Up(const CState_Player_Mount_Up& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_Up::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_IDLE);						
	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);			

	return S_OK;
}

HRESULT CState_Player_Mount_Up::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_Up::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySound(TEXT("Player_BikeOn.mp3"), CSound_Manager::CHANNEL_PLAYER);

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsGlow(true);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsGlow(true);
	}

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);
}

void CState_Player_Mount_Up::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	if (Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	m_pState_Handler->Set_JumpState(false);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_Up::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Mount_Up::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	//키 조정해야함
	if (KEY_DOWN('V'))
	{
		return true;
	}

	return false;
}


CState_Player_Mount_Up* CState_Player_Mount_Up::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_Up* pInstance = new CState_Player_Mount_Up(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_Up::Clone(void* pArg)
{
	CState_Player_Mount_Up* pInstance = new CState_Player_Mount_Up(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_Up::Free()
{
}
