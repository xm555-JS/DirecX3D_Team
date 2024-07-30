#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_Roll.h"
#include "GameInstance.h"
#include "Channel.h"
//#include "Imgui_Manager.h"
#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"

CState_Player_Sword_Attack_Roll::CState_Player_Sword_Attack_Roll(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_Roll::CState_Player_Sword_Attack_Roll(const CState_Player_Sword_Attack_Roll& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_Roll::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_A);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Sword_Attack_Roll::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_Roll::Enter(void* pArg)
{
	 MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_Roll_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(),0.5f);

	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Attack_Roll::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(200.f, 250.f), 12, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree0");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(200.f, 250.f), 32, 70, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Sword_Hit_Degree0");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200.f, 250.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.02f, 0.4f, 0.2f));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(200.f, 250.f)));
		//IMPL_ONCE(m_bShaked1, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.05f, 0.24f, 0.06f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (55 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (8 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Attack_Dash_Attack_B_1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	if (16 <= Get_CurKeyFrame())
	{
		_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
		ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
		m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);
		m_pColCom->Update(ColMatrix);
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Attack_Dash_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Attack_Dash_Attack_A", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, ""));
	}

	if (30 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_Roll_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
		ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
		m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 6.f);
		m_pColCom->Update(ColMatrix);
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Attack_B2_M", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 2.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
		IMPL_ONCE(m_bEffect4, EFFECT_POS("Attack_Dash_Attack_A_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 2.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Attack_Roll::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Attack_Roll::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (STATE_PLAYER_ROLL_FORWORD == pHandler->Get_CurStateType() &&
		KEY_DOWN(VK_LBUTTON))
		return true;

	return false;
}


CState_Player_Sword_Attack_Roll* CState_Player_Sword_Attack_Roll::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_Roll* pInstance = new CState_Player_Sword_Attack_Roll(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_Roll::Clone(void* pArg)
{
	CState_Player_Sword_Attack_Roll* pInstance = new CState_Player_Sword_Attack_Roll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_Roll::Free()
{
}
