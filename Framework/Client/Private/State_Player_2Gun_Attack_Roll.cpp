#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Roll.h"
#include "GameInstance.h"
#include "Channel.h"
//#include "Imgui_Manager.h"
#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"
#include "Channel.h"

CState_Player_2Gun_Attack_Roll::CState_Player_2Gun_Attack_Roll(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Roll::CState_Player_2Gun_Attack_Roll(const CState_Player_2Gun_Attack_Roll& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Roll::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_AIR_A);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Roll::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Roll::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	_matrix OffsetMatrix = XMMatrixScaling(0.7f, 0.7f, 0.7f);
	EFFECT_EXCUTE("Player_Gun_Attack_Foot", OffsetMatrix, 2.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL);

}

void CState_Player_2Gun_Attack_Roll::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(300.f, 360.f), 20, 50, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Gun_Hit");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(300.f, 360.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.045f, 0.2f, 0.2f));
	}

	if (16 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_Roll.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Gun_Attack_Foot2", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (20 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& !m_bJump)
	{
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		m_pState_Handler->Get_Player()->Get_Transform()->Jump(7.5f, pPlayerNavi);
		m_bJump = true;
	}

	if (55 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Roll::Exit(void* pArg)
{
	m_bJump = false;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Roll::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (STATE_PLAYER_ROLL_FORWORD == pHandler->Get_CurStateType() &&
		KEY_DOWN(VK_LBUTTON))
		return true;

	return false;
}


CState_Player_2Gun_Attack_Roll* CState_Player_2Gun_Attack_Roll::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Roll* pInstance = new CState_Player_2Gun_Attack_Roll(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Roll::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Roll* pInstance = new CState_Player_2Gun_Attack_Roll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Roll::Free()
{
}
