#include "stdafx.h"
#include "..\Public\State_Player_Jump_A.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Jump_A::CState_Player_Jump_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Jump_A::CState_Player_Jump_A(const CState_Player_Jump_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Jump_A::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_LOOP);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_B);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	//m_ReserveStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_Jump_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Jump_A::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.5f;

	USEGAMEINSTANCE;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(7.5f, pPlayerNavi);
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

}

void CState_Player_Jump_A::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Jump_A.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Jump_A::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Jump_A::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Jump_A* CState_Player_Jump_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Jump_A* pInstance = new CState_Player_Jump_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Jump_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Jump_A::Clone(void* pArg)
{
	CState_Player_Jump_A* pInstance = new CState_Player_Jump_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Jump_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Jump_A::Free()
{
}
