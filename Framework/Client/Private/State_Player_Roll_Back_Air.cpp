#include "stdafx.h"
#include "..\Public\State_Player_Roll_Back_Air.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"
#include "Weapon_ShieldAxe.h"

#include "HIerarchyNode.h"
#include "Effect_Manager.h"

CState_Player_Roll_Back_Air::CState_Player_Roll_Back_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Roll_Back_Air::CState_Player_Roll_Back_Air(const CState_Player_Roll_Back_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Roll_Back_Air::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Player_Roll_Back_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Roll_Back_Air::Enter(void* pArg)
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
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_HAND_ROLL_BACK_AIR);
	else
	{
		switch (iCurWeapon)
		{
		case WEAPON_SWORD:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SWORD_ROLL_BACK_AIR);
			break;
		case WEAPON_2GUN:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_2GUN_ROLL_BACK_AIR);
			break;
		case WEAPON_SHIELDAXE:
			if (((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Get_WeaponType() == 0)
				m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_SHIELD_ROLL_BACK_AIR);
			else
				m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_AXE_ROLL_BACK_AIR);
			break;
		case WEAPON_CUBE:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_CUBE_ROLL_BACK_AIR);
			break;
		default:
			m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_HAND_ROLL_BACK_AIR);
			break;
		}
	}

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

	m_pState_Handler->Set_CurState(m_pTargetState);
	m_pTargetState->Reset_EndCurState();
	m_pState_Handler->Set_CurStateType(m_pTargetState->Get_State());
	m_pTargetModel->Set_CurrentAnimation(m_pState_Handler->Get_AnimationIdx(m_pTargetState->Get_State()));
	m_pTargetState->Enter(pArg);
}

void CState_Player_Roll_Back_Air::Tick(_float fTimeDelta)
{

	//if (16 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	//{
	//	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	//	pPlayerTransform->Set_YFixState(false);
	//}
	USEGAMEINSTANCE;

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (40 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	//Effect
	if (3 <= Get_CurKeyFrame() && 16 >= Get_CurKeyFrame())
	{
		if (!(Get_CurKeyFrame() % 4))
		{
			IMPL_ONCE(m_bEffect0, EFFECT_POS("Dash_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
			RESET_ONCE(m_bEffect0);
		}

		if (!(Get_CurKeyFrame() % 8))
		{
			IMPL_ONCE(m_bEffect1, EFFECT_POS("Dash_M", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-0.3, 0.3), 0.f, 0.f, 1.f)));
			RESET_ONCE(m_bEffect1);
		}
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Roll_Back_Air::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	__super::Exit();
}

_bool CState_Player_Roll_Back_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler))
	{
		return true;
	}

	return false;
}


CState_Player_Roll_Back_Air* CState_Player_Roll_Back_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Roll_Back_Air* pInstance = new CState_Player_Roll_Back_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Roll_Back_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Roll_Back_Air::Clone(void* pArg)
{
	CState_Player_Roll_Back_Air* pInstance = new CState_Player_Roll_Back_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Roll_Back_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Roll_Back_Air::Free()
{
}
