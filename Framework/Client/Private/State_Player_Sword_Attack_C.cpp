#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_C.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Sword_Attack_C::CState_Player_Sword_Attack_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_C::CState_Player_Sword_Attack_C(const CState_Player_Sword_Attack_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_D);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Sword_Attack_C::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_C::Enter(void* pArg)
{
	// MGR(CSound_Manager)->PlaySound(TEXT("Player_SwordAttack_C_1.wav"), CSound_Manager::CHANNEL_PLAYER, 0.5f);

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Attack_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]) * 0.5f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100, 120), 30, 40, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree90");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(100, 120)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.02f, 0.1f, 0.1f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	if (KEY_PRESSING(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_DOWN(VK_SPACE))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_SPACE);
	}


	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		37 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (25 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_C_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Attack_C", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 2.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Attack_C::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	__super::Exit();
}

_bool CState_Player_Sword_Attack_C::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Sword_Attack_C* CState_Player_Sword_Attack_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_C* pInstance = new CState_Player_Sword_Attack_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_C::Clone(void* pArg)
{
	CState_Player_Sword_Attack_C* pInstance = new CState_Player_Sword_Attack_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_C::Free()
{
}
