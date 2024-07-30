#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_Skill_Air_A.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Axe_Attack_Skill_Air_A::CState_Player_Axe_Attack_Skill_Air_A(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_Skill_Air_A::CState_Player_Axe_Attack_Skill_Air_A(const CState_Player_Axe_Attack_Skill_Air_A& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_Skill_Air_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_END);
	m_ReserveStates.push_back(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_LOOP);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_Skill_Air_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_Skill_Air_A::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(7.5f, pPlayerNavi);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	EFFECT_EXCUTE("Player_Shield_Attack_D2_1", XMMatrixIdentity(), 0.5f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");
	EFFECT_EXCUTE("Player_Shield_Attack_D3", XMMatrixIdentity(), 0.5f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");
}

void CState_Player_Axe_Attack_Skill_Air_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Hit_ShieldAxe");
	//m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Skill_Normal.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));


	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_Skill_Air_A::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_Skill_Air_A::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1') && Enable_Use_Stamina(pHandler))
	{
		return true;
	}

	return false;
}


CState_Player_Axe_Attack_Skill_Air_A* CState_Player_Axe_Attack_Skill_Air_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_Skill_Air_A* pInstance = new CState_Player_Axe_Attack_Skill_Air_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_Skill_Air_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_Skill_Air_A::Clone(void* pArg)
{
	CState_Player_Axe_Attack_Skill_Air_A* pInstance = new CState_Player_Axe_Attack_Skill_Air_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_Skill_Air_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_Skill_Air_A::Free()
{
}
