#include "stdafx.h"
#include "..\Public\State_Player_Attack_Roll.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Imgui_Manager.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

CState_Player_Attack_Roll::CState_Player_Attack_Roll(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Attack_Roll::CState_Player_Attack_Roll(const CState_Player_Attack_Roll& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Attack_Roll::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Attack_Roll::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Attack_Roll::Enter(void* pArg)
{
	__super::Enter(pArg);
	_uint iCurWeapon = ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeapon();

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL() && iCurWeapon != WEAPON_2GUN)
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsGlow(false);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR() && iCurWeapon != WEAPON_2GUN)
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsGlow(false);
	}

	switch (iCurWeapon)
	{
	case WEAPON_SWORD:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_ATTACK_ROLL);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_ATTACK_ROLL);
		break;
	case WEAPON_SHIELDAXE:
		if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_ATTACK_A);
		else
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_ATTACK_ROLL);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ATTACK_B);
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

void CState_Player_Attack_Roll::Tick(_float fTimeDelta)
{
}

void CState_Player_Attack_Roll::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Attack_Roll::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Attack_Roll* CState_Player_Attack_Roll::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Attack_Roll* pInstance = new CState_Player_Attack_Roll(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Attack_Roll::Clone(void* pArg)
{
	CState_Player_Attack_Roll* pInstance = new CState_Player_Attack_Roll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Attack_Roll::Free()
{
}
