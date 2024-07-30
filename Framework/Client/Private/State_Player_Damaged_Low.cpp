#include "stdafx.h"
#include "..\Public\State_Player_Damaged_Low.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Player.h"

#include "Channel.h"

#include "Effect_Manager.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Damaged_Low::CState_Player_Damaged_Low(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Damaged_Low::CState_Player_Damaged_Low(const CState_Player_Damaged_Low& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Damaged_Low::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Damaged_Low::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	return S_OK;
}

void CState_Player_Damaged_Low::Enter(void* pArg)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Damaged_Low.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = false;
	m_fAnimSpeed = 2.f;
}

void CState_Player_Damaged_Low::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//약공은 하지말까?
	IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 2.f, 0.f, 0.f), 0.015f, 0.15f, 0.01f));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fCurDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge;
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;

	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& fCurDashGauge > fDashGauge / 3)
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_RBUTTON);
	}


	if (m_pState_Handler->Get_KeyInput() && 
		5 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Damaged", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Damaged_Low::Exit(void* pArg)
{
	__super::Exit();
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Damaged(0, false);
}

_bool CState_Player_Damaged_Low::Check_Condition(CState_Handler* pHandler)
{
	return	((CPlayer*)pHandler->Get_Player())->Get_Damaged(0);
}


CState_Player_Damaged_Low* CState_Player_Damaged_Low::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Damaged_Low* pInstance = new CState_Player_Damaged_Low(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Damaged_Low");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Damaged_Low::Clone(void* pArg)
{
	CState_Player_Damaged_Low* pInstance = new CState_Player_Damaged_Low(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Damaged_Low");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Damaged_Low::Free()
{
}
