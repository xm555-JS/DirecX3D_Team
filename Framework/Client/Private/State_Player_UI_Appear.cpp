#include "stdafx.h"
#include "..\Public\State_Player_UI_Appear.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Player.h"
#include "Weapon.h"
#include "UI_Equip_Button.h"
#include "UI_Equip_Icon.h"

CState_Player_UI_Appear::CState_Player_UI_Appear(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_UI_Appear::CState_Player_UI_Appear(const CState_Player_UI_Appear& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_UI_Appear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Player_UI_Appear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_UI_Appear::Enter(void* pArg)
{
	__super::Enter(pArg);
	USEGAMEINSTANCE;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	CUI_Equip_Button* pButton = (CUI_Equip_Button*)pGameInstance->Get_Object(m_pState_Handler->Get_Player()->Get_CurLevel(), ID_UINT(UI, UI_EQUIP_BUTTON));

	_uint iCurWeapon = ((CUI_Equip_Icon*)pButton->Get_UI_SelectWeapon())->Get_Selected_Weapon();
	
	switch (iCurWeapon)
	{
	case WEAPON_SWORD:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_UI_SWORD_APPEAR);
		break;
	case WEAPON_2GUN:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_UI_2GUN_APPEAR);
		break;
	case WEAPON_SHIELDAXE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_UI_SHIELD_APPEAR);
		break;
	case WEAPON_CUBE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_UI_CUBE_APPEAR);
		break;
	case WEAPON_NONE:
		m_pTargetState = m_pState_Handler->Get_State(STATE_PLAYER_UI_NONE_APPEAR);
		break;
	default:
		__debugbreak();
		break;
	}
	((CPlayer*)m_pState_Handler->Get_Player())->SetUp_UI_Weapon(iCurWeapon);

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

	m_pState_Handler->Set_CurState(m_pTargetState);
	m_pTargetState->Reset_EndCurState();
	m_pState_Handler->Set_CurStateType(m_pTargetState->Get_State());
	m_pTargetModel->Set_CurrentAnimation(m_pState_Handler->Get_AnimationIdx(m_pTargetState->Get_State()));
	m_pTargetState->Enter(pArg);
}

void CState_Player_UI_Appear::Tick(_float fTimeDelta)
{
}

void CState_Player_UI_Appear::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_UI_Appear::Check_Condition(CState_Handler* pHandler)
{
	// Equip창 안에있고, 선택된 슬롯이 바뀌거나 슬롯에 있는 무기가 바뀌면
	USEGAMEINSTANCE;
	if (((CPlayer*)pHandler->Get_Player())->Get_InEquip_Window()
		&& ((CPlayer*)pHandler->Get_Player())->Get_Change_SlotWeapon())
	{
		return true;
	}

	return false;
}


CState_Player_UI_Appear* CState_Player_UI_Appear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_UI_Appear* pInstance = new CState_Player_UI_Appear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_UI_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_UI_Appear::Clone(void* pArg)
{
	CState_Player_UI_Appear* pInstance = new CState_Player_UI_Appear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_UI_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_UI_Appear::Free()
{
}
