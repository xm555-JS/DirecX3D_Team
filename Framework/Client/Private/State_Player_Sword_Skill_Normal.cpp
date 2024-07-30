#include "stdafx.h"
#include "..\Public\State_Player_Sword_Skill_Normal.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Sword_Skill_Normal::CState_Player_Sword_Skill_Normal(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Skill_Normal::CState_Player_Sword_Skill_Normal(const CState_Player_Sword_Skill_Normal& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Skill_Normal::Initialize_Prototype()
{
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

HRESULT CState_Player_Sword_Skill_Normal::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Player_Sword_Skill_Normal::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());


}

void CState_Player_Sword_Skill_Normal::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 2.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}


	if (120 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (5 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Sword_Skill_Normal_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, -1.f, 0.f, 0.f), 0.04f, 0.45f, 0.45f));
	}
	if (20 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Sword_Skill_Normal_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	if (32 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_3.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Player_Sword_Skill_Normal_C", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	if (45 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_4.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Player_Sword_Skill_Normal_D", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}

	if (59 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_5.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect4, EFFECT_POS("Player_Sword_Skill_Normal_E", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
	}
	if (93 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Normal_6.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		
		_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
		ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]);
		m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);
		m_pColCom->Update(ColMatrix);
		IMPL_ONCE(m_bEffect5, EFFECT_POS("Player_Sword_Skill_Normal_F", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));
		IMPL_ONCE(m_bShaked4,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.045f, 0.5f, 0.5f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.005f, 0.f, 0.0025f, 0.f), 0.4f, 0.4f);
		);
	}

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 7, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree0");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 27, 35, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1,"Player_Sword_Hit_Degree-45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 38, 45, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Sword_Hit_Degree45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 49, 55, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Sword_Hit_Degree-45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 66, 74, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Sword_Hit_Degree45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 100, 120, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit5, "Player_Sword_Hit_Degree45");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.03f, 0.06f, 0.03f));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked1, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 1.f, 0.f, 0.f), 0.03f, 0.06f, 0.03f));
	}
	if (m_bHit2)
	{
		IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge2, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked2, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.03f, 0.06f, 0.03f));
	}
	if (m_bHit3)
	{
		IMPL_ONCE(m_bSound[13], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge3, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked3, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 1.f, 0.f, 0.f), 0.03f, 0.06f, 0.03f));
	}
	if (m_bHit4)
	{
		IMPL_ONCE(m_bSound[14], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge4, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked4, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 1.f, 0.f, 0.f), 0.03f, 0.06f, 0.03f));
	}
	if (m_bHit5)
	{
		IMPL_ONCE(m_bSound[15], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge5, m_pPlayer->Set_SkillGauge(fRandom(230.f, 270.f)));
		//IMPL_ONCE(m_bShaked5, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 1.f, 0.f, 0.f), 0.05f, 0.15f, 0.03f));
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Skill_Normal::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Skill_Normal::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1'))
	{
		return true;
	}

	return false;
}


CState_Player_Sword_Skill_Normal* CState_Player_Sword_Skill_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Skill_Normal* pInstance = new CState_Player_Sword_Skill_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Skill_Normal::Clone(void* pArg)
{
	CState_Player_Sword_Skill_Normal* pInstance = new CState_Player_Sword_Skill_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Skill_Normal::Free()
{
}
