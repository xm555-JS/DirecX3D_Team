#include "stdafx.h"
#include "..\Public\State_Player_UI_Shield_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Player.h"
#include "Weapon.h"


CState_Player_UI_Shield_Idle::CState_Player_UI_Shield_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_UI_Shield_Idle::CState_Player_UI_Shield_Idle(const CState_Player_UI_Shield_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_UI_Shield_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_UI_APPEAR);
	m_ForceStates.push_back(STATE_PLAYER_UI_SHIELD_IDLE);

	return S_OK;
}

HRESULT CState_Player_UI_Shield_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_UI_Shield_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;

	/*if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
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
	}*/
}

void CState_Player_UI_Shield_Idle::Tick(_float fTimeDelta)
{
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_UI_Shield_Idle::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_UI_Shield_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Player_UI_Shield_Idle* CState_Player_UI_Shield_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_UI_Shield_Idle* pInstance = new CState_Player_UI_Shield_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_UI_Shield_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_UI_Shield_Idle::Clone(void* pArg)
{
	CState_Player_UI_Shield_Idle* pInstance = new CState_Player_UI_Shield_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_UI_Shield_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_UI_Shield_Idle::Free()
{
}
