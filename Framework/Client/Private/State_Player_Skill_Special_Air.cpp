#include "stdafx.h"
#include "..\Public\State_Player_Skill_Special_Air.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"
#include "Weapon.h"

CState_Player_Skill_Special_Air::CState_Player_Skill_Special_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Skill_Special_Air::CState_Player_Skill_Special_Air(const CState_Player_Skill_Special_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Skill_Special_Air::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Skill_Special_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skill_Special_Air::Enter(void* pArg)
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

	// 무기 바뀌는거 셋팅해야함
	switch (iCurWeapon)
	{
	case WEAPON_SWORD:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_SKILL_SPECIAL_A);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_SKILL_SPECIAL_AIR);
		break;
	case WEAPON_SHIELDAXE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_ATTACK_SKILL_SPEAICAL);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL_AIR);
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

	m_pPlayer->Set_UsedSkill_S();
}

void CState_Player_Skill_Special_Air::Tick(_float fTimeDelta)
{
	//m_pTargetState->Tick(fTimeDelta);
}

void CState_Player_Skill_Special_Air::Exit(void* pArg)
{
	//m_pTargetState->Exit(pArg);
	__super::Exit();
}

_bool CState_Player_Skill_Special_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

#ifdef _DEBUG
	if (KEY_DOWN('0'))
	{
		return true;
	}
#endif

	if (((CPlayer*)pHandler->Get_Player())->Get_ChangeWeapon() && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput() 
		&& ((CPlayer*)pHandler->Get_Player())->Get_AbleSkill_S())
	{
		((CPlayer*)pHandler->Get_Player())->Reset_WeaponChange();
		return true;
	}

	((CPlayer*)pHandler->Get_Player())->Reset_WeaponChange();
	return false;
}


CState_Player_Skill_Special_Air* CState_Player_Skill_Special_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skill_Special_Air* pInstance = new CState_Player_Skill_Special_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skill_Special_Air::Clone(void* pArg)
{
	CState_Player_Skill_Special_Air* pInstance = new CState_Player_Skill_Special_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skill_Special_Air::Free()
{
}
