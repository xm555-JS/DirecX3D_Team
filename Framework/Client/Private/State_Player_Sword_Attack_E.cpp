#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_E.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Sword_Attack_E::CState_Player_Sword_Attack_E(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_E::CState_Player_Sword_Attack_E(const CState_Player_Sword_Attack_E& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_E::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Sword_Attack_E::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_E::Enter(void* pArg)
{

	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Attack_E::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 2.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
	}


	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		68 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (10 <= Get_CurKeyFrame())
	{
		_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
		ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
		m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 6.f);
		m_pColCom->Update(ColMatrix);
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_E_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Attack_E1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));

	}

	//if (25 <= Get_CurKeyFrame())
	//	IMPL_ONCE(m_bEffect1, EFFECT_POS("Attack_E2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));

#if 1
	if (25 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_E_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Player_Sword_Attack_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 1.7f, 1.f, 1.f),
				XMVectorSet(XMConvertToRadians(180.f), XMConvertToRadians(90.f), 0.f, 0.f),
				1.5f
			)
		);
	}
#endif // 0


	//if (30 <= Get_CurKeyFrame())
	//	IMPL_ONCE(m_bEffect2, EFFECT_POS("Attack_E3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	if (30 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_E_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Player_Sword_Attack_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 1.7f, 1.f, 1.f),
				XMVectorSet(XMConvertToRadians(200.f), XMConvertToRadians(90.f), 0.f, 0.f),
				1.5f
			)
		);
	}

#if 1
	if (37 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_E_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
		ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
		m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 8.f);
		m_pColCom->Update(ColMatrix);
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Attack_E4", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));

		//IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.1f, 0.8f, 0.04f));
	}
#endif // 0


	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(200, 220), 12, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree90");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(350, 420), 28, 70, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Sword_Hit_Degree0");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200, 220)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.04f, 0.2f, 0.2f));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(350, 420)));
		//IMPL_ONCE(m_bShaked1, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.05f, 0.15f, 0.03f));
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Attack_E::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	__super::Exit();
}

_bool CState_Player_Sword_Attack_E::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Sword_Attack_E* CState_Player_Sword_Attack_E::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_E* pInstance = new CState_Player_Sword_Attack_E(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_E::Clone(void* pArg)
{
	CState_Player_Sword_Attack_E* pInstance = new CState_Player_Sword_Attack_E(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_E::Free()
{
}
