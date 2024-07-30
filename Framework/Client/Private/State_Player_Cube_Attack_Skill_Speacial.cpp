#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Skill_Special.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

#include "Imgui_Manager.h"

CState_Player_Cube_Attack_Skill_Special::CState_Player_Cube_Attack_Skill_Special(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Skill_Special::CState_Player_Cube_Attack_Skill_Special(const CState_Player_Cube_Attack_Skill_Special& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Skill_Special::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_A);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Skill_Special::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Player_Cube_Attack_Skill_Special::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Cube_Attack_Skill_Special::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 10.f);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 70, 72, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 73, 75, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 76, 78, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 79, 82, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 83, 86, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(180.f, 230.f), 87, 90, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit5, "Player_Hit_Cube");

	if (0 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0,
			EFFECT_EXCUTE("Player_Cube_Attack_Jump1", XMMatrixIdentity(), 2.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");
		);
	}
	if(60 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);
		IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_IceForm.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.3f, fTimeDelta);
	}
	
	if (60 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Charge_Jump.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.f, 0.015f, 0.0075f, 0.f), 0.4f, 0.4f);
			pGameInstance->Add_Camera_Shaking({ 1.f,1.f,0.f,0.f }, 0.042f, 0.45f, 0.45f););
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (120 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D') ||
		KEY_PRESSING(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	if (KEY_DOWN(VK_LBUTTON))
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
		95 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Skill_Special::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);

	m_bChange = false;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Skill_Special::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1'))
	{
		return true;
	}

	return false;
}


CState_Player_Cube_Attack_Skill_Special* CState_Player_Cube_Attack_Skill_Special::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Skill_Special* pInstance = new CState_Player_Cube_Attack_Skill_Special(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Skill_Special::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Skill_Special* pInstance = new CState_Player_Cube_Attack_Skill_Special(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Skill_Special::Free()
{
}
