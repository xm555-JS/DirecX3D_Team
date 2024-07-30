#include "stdafx.h"
#include "..\Public\State_Player_Axe_Roll_Back_Air.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"

#include "HIerarchyNode.h"
#include "Effect_Manager.h"

CState_Player_Axe_Roll_Back_Air::CState_Player_Axe_Roll_Back_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Roll_Back_Air::CState_Player_Axe_Roll_Back_Air(const CState_Player_Axe_Roll_Back_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Roll_Back_Air::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);

	return S_OK;
}

HRESULT CState_Player_Axe_Roll_Back_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Axe_Roll_Back_Air::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_bEndCurState = false;
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	//m_fAnimSpeed = 4.f;
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge -= fDashGauge / 3;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
}

void CState_Player_Axe_Roll_Back_Air::Tick(_float fTimeDelta)
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

	if (KEY_DOWN(VK_SPACE))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_SPACE);
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
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

void CState_Player_Axe_Roll_Back_Air::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	__super::Exit();
}

_bool CState_Player_Axe_Roll_Back_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler))
	{
		return true;
	}

	return false;
}


CState_Player_Axe_Roll_Back_Air* CState_Player_Axe_Roll_Back_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Roll_Back_Air* pInstance = new CState_Player_Axe_Roll_Back_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Roll_Back_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Roll_Back_Air::Clone(void* pArg)
{
	CState_Player_Axe_Roll_Back_Air* pInstance = new CState_Player_Axe_Roll_Back_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Roll_Back_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Roll_Back_Air::Free()
{
}
