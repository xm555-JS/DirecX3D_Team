#include "stdafx.h"
#include "..\Public\State_Player_Attack_Jump_Charge.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

CState_Player_Attack_Jump_Charge::CState_Player_Attack_Jump_Charge(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Attack_Jump_Charge::CState_Player_Attack_Jump_Charge(const CState_Player_Attack_Jump_Charge& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Attack_Jump_Charge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Attack_Jump_Charge::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Attack_Jump_Charge::Enter(void* pArg)
{
	__super::Enter(pArg);
	_uint iCurWeapon = ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeapon();

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsGlow(false);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsGlow(false);
	}

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

	switch (iCurWeapon)
	{
	case WEAPON_SWORD:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_A);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_A);
		break;
	case WEAPON_SHIELDAXE:
		if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_A);
		else
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_A);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_BEGIN);
		break;
	default:
		__debugbreak();
		break;
	}

	m_pState_Handler->Set_CurState(m_pTargetState);
	m_pTargetState->Reset_EndCurState();
	m_pState_Handler->Set_CurStateType(m_pTargetState->Get_State());
	m_pTargetModel->Set_CurrentAnimation(m_pState_Handler->Get_AnimationIdx(m_pTargetState->Get_State()));
	m_pTargetState->Enter(pArg);
}

void CState_Player_Attack_Jump_Charge::Tick(_float fTimeDelta)
{
}

void CState_Player_Attack_Jump_Charge::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Attack_Jump_Charge::Check_Condition(CState_Handler* pHandler)
{
	if(0.15f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Attack_Jump_Charge* CState_Player_Attack_Jump_Charge::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Attack_Jump_Charge* pInstance = new CState_Player_Attack_Jump_Charge(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Attack_Jump_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Attack_Jump_Charge::Clone(void* pArg)
{
	CState_Player_Attack_Jump_Charge* pInstance = new CState_Player_Attack_Jump_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Attack_Jump_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Attack_Jump_Charge::Free()
{
}
