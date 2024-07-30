#include "stdafx.h"
#include "..\Public\State_Player_Jump_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Channel.h"
#include "Player.h"
#include "Weapon.h"

#include "Imgui_Manager.h"

CState_Player_Jump_End::CState_Player_Jump_End(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Jump_End::CState_Player_Jump_End(const CState_Player_Jump_End& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Jump_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);


	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);

	return S_OK;
}

HRESULT CState_Player_Jump_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Jump_End::Enter(void* pArg)
{

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.5f;

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);
	m_pPlayer->Set_Target(nullptr);
}

void CState_Player_Jump_End::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (5 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Jump_End.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

		m_bEndCurState = true;
	}


	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		//_float	fRunSpeed = fPlayerSpeed * DEBUGFLOAT4(0).y;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

		m_pState_Handler->Get_Player()->Get_Transform()->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Jump_End::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	m_pState_Handler->Set_JumpState(false);
	__super::Exit();
}

_bool CState_Player_Jump_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;


	//if (KEY_NONE('W') &&
	//	KEY_NONE('A') &&
	//	KEY_NONE('S') &&
	//	KEY_NONE('D'))
	//{
		CTransform*		pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
		CNavigation*	pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
		_float			fHightDiff = 0.f;
		pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

		if (fHightDiff < 1.f)
			return true;
	//}

	return false;
}


CState_Player_Jump_End* CState_Player_Jump_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Jump_End* pInstance = new CState_Player_Jump_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Jump_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Jump_End::Clone(void* pArg)
{
	CState_Player_Jump_End* pInstance = new CState_Player_Jump_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Jump_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Jump_End::Free()
{
}
