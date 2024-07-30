#include "stdafx.h"
#include "..\Public\State_Player_Skill_Normal_Air.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

CState_Player_Skill_Normal_Air::CState_Player_Skill_Normal_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Skill_Normal_Air::CState_Player_Skill_Normal_Air(const CState_Player_Skill_Normal_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Skill_Normal_Air::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Skill_Normal_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skill_Normal_Air::Enter(void* pArg)
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
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_SKILL_NORMAL_AIR);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_SKILL_NORMAL_AIR);
		break;
	case WEAPON_SHIELDAXE:
		if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_BEGIN);
		else
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR);
		break;
	default:
		__debugbreak();
		break;
	}
	
	((CPlayer*)m_pState_Handler->Get_Player())->Set_TimeSkill_N((WEAPON_TYPE)iCurWeapon);

	m_pState_Handler->Set_CurState(m_pTargetState);
	m_pTargetState->Reset_EndCurState();
	m_pState_Handler->Set_CurStateType(m_pTargetState->Get_State());
	m_pTargetModel->Set_CurrentAnimation(m_pState_Handler->Get_AnimationIdx(m_pTargetState->Get_State()));
	m_pTargetState->Enter(pArg);

}

void CState_Player_Skill_Normal_Air::Tick(_float fTimeDelta)
{
	//m_pTargetState->Tick(fTimeDelta);
}

void CState_Player_Skill_Normal_Air::Exit(void* pArg)
{
	//m_pTargetState->Exit(pArg);
	__super::Exit();
}

_bool CState_Player_Skill_Normal_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1') && ((CPlayer::PLAYERDESC*)((CUnit*)pHandler->Get_Player())->Get_UnitCurInfo())->bAbleSkill_N)
	{
		return true;
	}

	return false;
}


CState_Player_Skill_Normal_Air* CState_Player_Skill_Normal_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skill_Normal_Air* pInstance = new CState_Player_Skill_Normal_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skill_Normal_Air::Clone(void* pArg)
{
	CState_Player_Skill_Normal_Air* pInstance = new CState_Player_Skill_Normal_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skill_Normal_Air::Free()
{
}
