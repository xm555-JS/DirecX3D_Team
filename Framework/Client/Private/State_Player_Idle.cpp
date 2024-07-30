#include "stdafx.h"
#include "..\Public\State_Player_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Npc_Shirley.h"
#include "Player.h"
#include "Weapon.h"
#include "Weapon_ShieldAxe.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Idle::CState_Player_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Idle::CState_Player_Idle(const CState_Player_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Idle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
	_uint iCurWeapon = ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeapon();

	m_bEndCurState = true;
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
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

	switch (iCurWeapon)
	{
	case WEAPON_SWORD:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_IDLE);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_IDLE);
		break;
	case WEAPON_SHIELDAXE:
		if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_IDLE);
		else
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_IDLE);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_IDLE);
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

void CState_Player_Idle::Tick(_float fTimeDelta)
{
}

void CState_Player_Idle::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		_float4 vPos;
		XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
		if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
			return false;

		return true;
	}

	return false;
}


CState_Player_Idle* CState_Player_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Idle* pInstance = new CState_Player_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Idle::Clone(void* pArg)
{
	CState_Player_Idle* pInstance = new CState_Player_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Idle::Free()
{
}
