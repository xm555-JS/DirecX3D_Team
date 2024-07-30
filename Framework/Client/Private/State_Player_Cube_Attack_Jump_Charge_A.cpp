#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Jump_Charge_A.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Cube_Attack_Jump_Charge_A::CState_Player_Cube_Attack_Jump_Charge_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Jump_Charge_A::CState_Player_Cube_Attack_Jump_Charge_A(const CState_Player_Cube_Attack_Jump_Charge_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_LOOP);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_A::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Jump_Charge_A::Enter(void* pArg)
{
	__super::Enter(pArg);
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
	Use_StaminaGauge(0.1f);
}

void CState_Player_Cube_Attack_Jump_Charge_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Sword_Hit_Degree90");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	if (15 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);

		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Attack_Jump1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 1.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 2.f));
	}
	if (40 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Charge_Jump.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Attack_Jump3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 1.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 0.25f));
	}

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200, 220)));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Jump_Charge_A::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Jump_Charge_A::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Cube_Attack_Jump_Charge_A* CState_Player_Cube_Attack_Jump_Charge_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Jump_Charge_A* pInstance = new CState_Player_Cube_Attack_Jump_Charge_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Jump_Charge_A::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Jump_Charge_A* pInstance = new CState_Player_Cube_Attack_Jump_Charge_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Jump_Charge_A::Free()
{
}
