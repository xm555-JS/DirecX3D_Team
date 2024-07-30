#include "stdafx.h"
#include "..\Public\State_Player_Sword_Skill_Special_E.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_Sword_Skill_Special_E::CState_Player_Sword_Skill_Special_E(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Skill_Special_E::CState_Player_Sword_Skill_Special_E(const CState_Player_Sword_Skill_Special_E& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Skill_Special_E::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_Sword_Skill_Special_E::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Skill_Special_E::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	XMStoreFloat4x4(&m_ColMatrix, XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()) * XMMatrixTranslation(0.f, -2.f, 0.f));
}

void CState_Player_Sword_Skill_Special_E::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Update(XMLoadFloat4x4(&m_ColMatrix));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 10.f);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 33, 50, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree90");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 51, 55, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Sword_Hit_Degree45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 56, 60, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Sword_Hit_Degree0");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 61, 65, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Sword_Hit_Degree-45");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 66, 70, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Sword_Hit_Degree90");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(150.f, 250.f), 71, 75, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit5, "Player_Sword_Hit_Degree0");

	/*if (m_bHit3)
	{
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 0.f), _float4(0.f,0.02f,0.01f,0.f), 0.5f, 0.5f);
			pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f,0.f), 0.03f, 0.5f, 0.25f));
	}*/
	if (m_bHit0)
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	if (m_bHit1)
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	if (m_bHit2)
		IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	if (m_bHit3)
		IMPL_ONCE(m_bSound[13], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	if (m_bHit4)
		IMPL_ONCE(m_bSound[14], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	if (m_bHit5)
		IMPL_ONCE(m_bSound[15], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	if (38 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_E.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Attack_Jump_D", m_pState_Handler->Get_Owner()->Get_WorldMatrix()
			)
		);
	}

	if (41 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-0.5f, 2.f), _float4(0.f, 0.02f,  0.01f, 0.f), 0.5f, 0.5f);
		pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.03f, 0.5f, 0.2f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Skill_Special_E::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Skill_Special_E::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Sword_Skill_Special_E* CState_Player_Sword_Skill_Special_E::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Skill_Special_E* pInstance = new CState_Player_Sword_Skill_Special_E(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Skill_Special_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Skill_Special_E::Clone(void* pArg)
{
	CState_Player_Sword_Skill_Special_E* pInstance = new CState_Player_Sword_Skill_Special_E(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Skill_Special_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Skill_Special_E::Free()
{
}
