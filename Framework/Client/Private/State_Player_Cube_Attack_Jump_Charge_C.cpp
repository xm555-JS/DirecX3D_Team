#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Jump_Charge_C.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Cube_Attack_Jump_Charge_C::CState_Player_Cube_Attack_Jump_Charge_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Jump_Charge_C::CState_Player_Cube_Attack_Jump_Charge_C(const CState_Player_Cube_Attack_Jump_Charge_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_C::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Jump_Charge_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Cube_Attack_Jump_Charge_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Sword_Hit_Degree90");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Jump_Charge_C::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Jump_Charge_C::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Cube_Attack_Jump_Charge_C* CState_Player_Cube_Attack_Jump_Charge_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Jump_Charge_C* pInstance = new CState_Player_Cube_Attack_Jump_Charge_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Jump_Charge_C::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Jump_Charge_C* pInstance = new CState_Player_Cube_Attack_Jump_Charge_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Jump_Charge_C::Free()
{
}
