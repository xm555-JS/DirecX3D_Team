#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Skill_Special_Air.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"
#include "Player.h"

CState_Player_Cube_Attack_Skill_Special_Air::CState_Player_Cube_Attack_Skill_Special_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Skill_Special_Air::CState_Player_Cube_Attack_Skill_Special_Air(const CState_Player_Cube_Attack_Skill_Special_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Skill_Special_Air::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);//	,

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Skill_Special_Air::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Skill_Special_Air::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(8.5f, pPlayerNavi);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Cube_Attack_Skill_Special_Air::Tick(_float fTimeDelta)
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

	if (60 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);
		IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_IceForm.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.3f, fTimeDelta);
	}

	if (60 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Charge_Jump.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.f, 0.015f, 0.0075f, 0.f), 0.4f, 0.4f);
			pGameInstance->Add_Camera_Shaking({ 1.f,1.f,0.f,0.f }, 0.042f, 0.45f, 0.45f););
	}



	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Sword_Hit_Degree90");
	//m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Skill_Special_Air::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Skill_Special_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1') && Enable_Use_Stamina(pHandler))
	{
		return true;
	}

	return false;
}


CState_Player_Cube_Attack_Skill_Special_Air* CState_Player_Cube_Attack_Skill_Special_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Skill_Special_Air* pInstance = new CState_Player_Cube_Attack_Skill_Special_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Skill_Special_Air::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Skill_Special_Air* pInstance = new CState_Player_Cube_Attack_Skill_Special_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Skill_Special_Air::Free()
{
}
