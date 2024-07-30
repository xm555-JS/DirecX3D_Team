#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_Skill_Air_B.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Axe_Attack_Skill_Air_B::CState_Player_Axe_Attack_Skill_Air_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_Skill_Air_B::CState_Player_Axe_Attack_Skill_Air_B(const CState_Player_Axe_Attack_Skill_Air_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_Skill_Air_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_END);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_Skill_Air_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_Skill_Air_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	m_pState_Handler->Get_Player()->Get_Pos();
}

void CState_Player_Axe_Attack_Skill_Air_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Hit_ShieldAxe");
	//m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_Skill_Air_B::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_Skill_Air_B::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Axe_Attack_Skill_Air_B* CState_Player_Axe_Attack_Skill_Air_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_Skill_Air_B* pInstance = new CState_Player_Axe_Attack_Skill_Air_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_Skill_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_Skill_Air_B::Clone(void* pArg)
{
	CState_Player_Axe_Attack_Skill_Air_B* pInstance = new CState_Player_Axe_Attack_Skill_Air_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_Skill_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_Skill_Air_B::Free()
{
}
