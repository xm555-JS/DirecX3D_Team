#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_Jump_Charge_A.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Axe_Attack_Jump_Charge_A::CState_Player_Axe_Attack_Jump_Charge_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_Jump_Charge_A::CState_Player_Axe_Attack_Jump_Charge_A(const CState_Player_Axe_Attack_Jump_Charge_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_Jump_Charge_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_B);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_Jump_Charge_A::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_Jump_Charge_A::Enter(void* pArg)
{
	__super::Enter(pArg);
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Axe_Attack_Jump_Charge_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Hit_ShieldAxe");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);


	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_Jump_Charge_A::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_Jump_Charge_A::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Axe_Attack_Jump_Charge_A* CState_Player_Axe_Attack_Jump_Charge_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_Jump_Charge_A* pInstance = new CState_Player_Axe_Attack_Jump_Charge_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_Jump_Charge_A::Clone(void* pArg)
{
	CState_Player_Axe_Attack_Jump_Charge_A* pInstance = new CState_Player_Axe_Attack_Jump_Charge_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_Jump_Charge_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_Jump_Charge_A::Free()
{
}
