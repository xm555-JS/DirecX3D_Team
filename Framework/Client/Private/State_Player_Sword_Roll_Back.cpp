#include "stdafx.h"
#include "..\Public\State_Player_Sword_Roll_Back.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"

#include "HIerarchyNode.h"
#include "Effect_Manager.h"

CState_Player_Sword_Roll_Back::CState_Player_Sword_Roll_Back(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Roll_Back::CState_Player_Sword_Roll_Back(const CState_Player_Sword_Roll_Back& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Roll_Back::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_ROLL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_RUN);

	return S_OK;
}

HRESULT CState_Player_Sword_Roll_Back::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Roll_Back::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = false;
	m_fAnimSpeed = 3.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	m_fOldGravity = pPlayerTransform->Get_Gravity();
	pPlayerTransform->Set_Gravity(0.f);

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge -= fDashGauge / 3;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
}

void CState_Player_Sword_Roll_Back::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
	//	KEY_DOWN(VK_SPACE))
	//{
	//	m_bEndCurState = true;
	//}

	if (22 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

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

void CState_Player_Sword_Roll_Back::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_Gravity(m_fOldGravity);
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Sword_Roll_Back::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Sword_Roll_Back* CState_Player_Sword_Roll_Back::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Roll_Back* pInstance = new CState_Player_Sword_Roll_Back(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Roll_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Roll_Back::Clone(void* pArg)
{
	CState_Player_Sword_Roll_Back* pInstance = new CState_Player_Sword_Roll_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Roll_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Roll_Back::Free()
{
}
