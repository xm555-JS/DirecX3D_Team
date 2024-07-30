#include "stdafx.h"
#include "..\Public\State_Player_Axe_Skill_Normal.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

#include "Imgui_Manager.h"

CState_Player_Axe_Skill_Normal::CState_Player_Axe_Skill_Normal(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Skill_Normal::CState_Player_Axe_Skill_Normal(const CState_Player_Axe_Skill_Normal& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Skill_Normal::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SHIELD_ATTACK_A);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Axe_Skill_Normal::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Player_Axe_Skill_Normal::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Axe_Skill_Normal::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (0 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Skill_Normal.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			EFFECT_EXCUTE("Player_Shield_Attack_D2_1", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp")
		);

		IMPL_ONCE(m_bEffect2,
			EFFECT_EXCUTE("Player_Shield_Attack_D3", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp")
		);
	}

	if (45 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Hit_Ground.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Shield_Attack_Jump_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 0.5f, 1.f), // Pos
				XMVectorSet(0.f, 0.f, 0.f, 0.f), // Rot
				1.f,
				XMVectorSet(1.5f, 1.5f, 1.5f, 1.f) // Pos
			)
		);

		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.06f, 0.2f, 0.1f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.008f, 0.f, 0.004f, 0.f), 0.4f, 0.4f);
		);
	}

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 2.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (10 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& !m_bChange)
	{
		m_bChange = true;
		((CWeapon_ShieldAxe*)((CPlayer*)m_pState_Handler->Get_Player())->Get_Weapon(CPlayer::PLAYER_WEAPON_SHIELDAXE))->Set_ChangeWeapon();
	}


	if (120 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Skill_Normal::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_bChange = false;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Skill_Normal::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1'))
	{
		return true;
	}

	return false;
}


CState_Player_Axe_Skill_Normal* CState_Player_Axe_Skill_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Skill_Normal* pInstance = new CState_Player_Axe_Skill_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Skill_Normal::Clone(void* pArg)
{
	CState_Player_Axe_Skill_Normal* pInstance = new CState_Player_Axe_Skill_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Skill_Normal::Free()
{
}
