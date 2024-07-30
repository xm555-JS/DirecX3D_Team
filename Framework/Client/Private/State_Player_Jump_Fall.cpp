#include "stdafx.h"
#include "..\Public\State_Player_Jump_Fall.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Channel.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Jump_Fall::CState_Player_Jump_Fall(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Jump_Fall::CState_Player_Jump_Fall(const CState_Player_Jump_Fall& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Jump_Fall::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_LOOP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_B);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	//m_ReserveStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);

	return S_OK;
}

HRESULT CState_Player_Jump_Fall::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Jump_Fall::Enter(void* pArg)
{
	if (STATE_PLAYER_GLIDE_LOOP==m_pState_Handler->Get_PreStateType())
	{
		MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOff.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f);
	}
	__super::Enter(pArg);

	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 1.5f;

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(0.f, pPlayerNavi);
	m_pPlayer->Set_Target(nullptr);

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

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

}

void CState_Player_Jump_Fall::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

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

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Jump_Fall::Exit(void* pArg)
{

	m_fKeyPressTimeAcc = 0.f;
	// ���� Female�� ���
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Jump_Fall::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	if (STATE_PLAYER_GLIDE_UP == pHandler->Get_CurStateType() ||
		STATE_PLAYER_GLIDE_LOOP == pHandler->Get_CurStateType() ||
		STATE_PLAYER_GLIDE_LOOP_F == pHandler->Get_CurStateType() ||
		STATE_PLAYER_GLIDE_BEGIN_F == pHandler->Get_CurStateType())
	{
		if (KEY_DOWN('2'))
		{
			return true;
		}
	}
	else if (STATE_PLAYER_MOUNT_JUMP_START == pHandler->Get_CurStateType() ||
		STATE_PLAYER_MOUNT_DRIVE == pHandler->Get_CurStateType())
	{
		_float fJumpDiff = ((CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM))->Get_JumpYDiff();
		if (0.f >= fJumpDiff)
		{
			if (KEY_DOWN('V'))
			{
				return true;
			}
		}
	}
	else
	{
		_float fJumpDiff = ((CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM))->Get_JumpYDiff();
		if (0.f >= fJumpDiff)
		{
			return true;
		}
	}

	return false;
}


CState_Player_Jump_Fall* CState_Player_Jump_Fall::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Jump_Fall* pInstance = new CState_Player_Jump_Fall(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Jump_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Jump_Fall::Clone(void* pArg)
{
	CState_Player_Jump_Fall* pInstance = new CState_Player_Jump_Fall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Jump_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Jump_Fall::Free()
{
}
