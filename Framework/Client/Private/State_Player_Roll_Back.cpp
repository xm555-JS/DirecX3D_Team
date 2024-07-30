#include "stdafx.h"
#include "..\Public\State_Player_Roll_Back.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"
#include "Weapon_ShieldAxe.h"

#include "HIerarchyNode.h"
#include "Effect_Manager.h"

CState_Player_Roll_Back::CState_Player_Roll_Back(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Roll_Back::CState_Player_Roll_Back(const CState_Player_Roll_Back& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Roll_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_Roll_Back::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Roll_Back::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Dash.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f);

	__super::Enter(pArg);
	
	USEGAMEINSTANCE;
	pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.5f, 0.5f, 1.f, 0.03f);

	_uint iCurWeapon = ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeapon();

	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	m_pPlayer->Set_Evasion(true);

	if ((nullptr == m_pPlayer->Get_CurWeaponL() && nullptr == m_pPlayer->Get_CurWeaponR())
		|| (nullptr != m_pPlayer->Get_CurWeaponL() && m_pPlayer->Get_CurWeaponL()->Get_Dissolved())
		|| (nullptr != m_pPlayer->Get_CurWeaponR() && m_pPlayer->Get_CurWeaponR()->Get_Dissolved()))
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_HAND_ROLL_BACK);
	else
	{
		switch (iCurWeapon)
		{
		case WEAPON_SWORD:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_ROLL_BACK);
			break;
		case WEAPON_2GUN:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_ROLL_BACK);
			break;
		case WEAPON_SHIELDAXE:
			if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
				m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_ROLL_BACK);
			else
				m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_ROLL_BACK);
			break;
		case WEAPON_CUBE:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ROLL_BACK);
			break;
		default:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_HAND_ROLL_BACK);
			break;
		}
	}

	m_pState_Handler->Set_CurState(m_pTargetState);
	m_pTargetState->Reset_EndCurState();
	m_pState_Handler->Set_CurStateType(m_pTargetState->Get_State());
	m_pTargetModel->Set_CurrentAnimation(m_pState_Handler->Get_AnimationIdx(m_pTargetState->Get_State()));
	m_pTargetState->Enter(pArg);
}

void CState_Player_Roll_Back::Tick(_float fTimeDelta)
{

}

void CState_Player_Roll_Back::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Roll_Back::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Roll_Back* CState_Player_Roll_Back::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Roll_Back* pInstance = new CState_Player_Roll_Back(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Roll_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Roll_Back::Clone(void* pArg)
{
	CState_Player_Roll_Back* pInstance = new CState_Player_Roll_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Roll_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Roll_Back::Free()
{
}
