#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_Base_Charge.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Sword_Attack_Base_Charge::CState_Player_Sword_Attack_Base_Charge(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_Base_Charge::CState_Player_Sword_Attack_Base_Charge(const CState_Player_Sword_Attack_Base_Charge& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_Base_Charge::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_AIR_A);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Sword_Attack_Base_Charge::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_Base_Charge::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Attack_Base_Charge::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//if (6 < Get_CurKeyFrame())
	//	IMPL_ONCE(m_bEffect0, EFFECT_POS("Attack_Charge_Jump", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 1.5f));

	if (6 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_Charge.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(-0.8f, 2.2f, 0.5f, 1.f),
				XMVectorSet(XMConvertToRadians(90.f), XMConvertToRadians(90.f), 0.f, 0.f),
				1.5f
			)
		);
	}


	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f, 270.f), 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree90");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.028f, 0.2f, 0.2f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (7 < Get_CurKeyFrame() && !m_bJump)
	{
		m_fAnimSpeed = 1.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		m_pState_Handler->Get_Player()->Get_Transform()->Jump(8.f, pPlayerNavi);
		m_bJump = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Attack_Base_Charge::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_bJump = false;
	__super::Exit();
}

_bool CState_Player_Sword_Attack_Base_Charge::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Sword_Attack_Base_Charge* CState_Player_Sword_Attack_Base_Charge::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_Base_Charge* pInstance = new CState_Player_Sword_Attack_Base_Charge(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_Base_Charge::Clone(void* pArg)
{
	CState_Player_Sword_Attack_Base_Charge* pInstance = new CState_Player_Sword_Attack_Base_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_Base_Charge::Free()
{
}
