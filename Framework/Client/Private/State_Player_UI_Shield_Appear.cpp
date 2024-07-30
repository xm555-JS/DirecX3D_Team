#include "stdafx.h"
#include "..\Public\State_Player_UI_Shield_Appear.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Player.h"
#include "Weapon.h"

CState_Player_UI_Shield_Appear::CState_Player_UI_Shield_Appear(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_UI_Shield_Appear::CState_Player_UI_Shield_Appear(const CState_Player_UI_Shield_Appear& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_UI_Shield_Appear::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_UI_APPEAR);

	m_ReserveStates.push_back(STATE_PLAYER_UI_SHIELD_IDLE);

	return S_OK;
}

HRESULT CState_Player_UI_Shield_Appear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_UI_Shield_Appear::Enter(void* pArg)
{
	__super::Enter(pArg);
	
	m_fAnimSpeed = 2.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

}

void CState_Player_UI_Shield_Appear::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_UI_Shield_Appear::Exit(void* pArg)
{
	__super::Exit();
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
}

_bool CState_Player_UI_Shield_Appear::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (((CPlayer*)pHandler->Get_Player())->Get_InEquip_Window())
	{
		return true;
	}

	return false;
}

CState_Player_UI_Shield_Appear* CState_Player_UI_Shield_Appear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_UI_Shield_Appear* pInstance = new CState_Player_UI_Shield_Appear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_UI_Shield_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_UI_Shield_Appear::Clone(void* pArg)
{
	CState_Player_UI_Shield_Appear* pInstance = new CState_Player_UI_Shield_Appear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_UI_Shield_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_UI_Shield_Appear::Free()
{
}
