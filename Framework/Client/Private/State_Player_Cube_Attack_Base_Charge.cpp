#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Base_Charge.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"
#include "Player.h"

CState_Player_Cube_Attack_Base_Charge::CState_Player_Cube_Attack_Base_Charge(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Cube(pManager, eState)
{
}

CState_Player_Cube_Attack_Base_Charge::CState_Player_Cube_Attack_Base_Charge(const CState_Player_Cube_Attack_Base_Charge& rhs)
	: CState_Player_Cube(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Base_Charge::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Base_Charge::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Base_Charge::Enter(void* pArg)
{
	__super::Enter(pArg);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	m_iSound = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	EFFECT_EXCUTE("Player_Cube_Attack_Charge_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");
}

void CState_Player_Cube_Attack_Base_Charge::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (24 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);

	}

	if (24 < Get_CurKeyFrame() && 50 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Attack_Charge_M", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		m_fTimeAcc += fTimeDelta;
		if (0.1f < m_fTimeAcc)
		{
			IMPL_ONCE(m_bSound[m_iSound], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Charge_Base.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

			EFFECT_POS("Player_Cube_Attack1_Up_Middle-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-2.5f, 2.5f), 0.f, fRandom(-2.5f, 2.5f), 1.f)); //, XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.6f, 0.6f, 0.6f, 0.f));
			EFFECT_POS("Player_Cube_Attack2_Up_Middle-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-2.5f, 2.5f), 0.f, fRandom(-2.5f, 2.5f), 1.f)); //, XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.6f, 0.6f, 0.6f, 0.f));
			m_fTimeAcc = 0.f;
			++m_iSound;
		}
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 7.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(120.f, 150.f), 24, 30, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(120.f, 150.f), 31, 35, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(120.f, 150.f), 36, 40, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(120.f, 150.f), 41, 45, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(120.f, 150.f), 46, 50, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Hit_Cube");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(120.f, 150.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.04f, 0.17f, 0.17f));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(120.f, 150.f)));
	}
	if (m_bHit2)
	{
		IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge2, m_pPlayer->Set_SkillGauge(fRandom(120.f, 150.f)));
	}
	if (m_bHit3)
	{
		IMPL_ONCE(m_bSound[13], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge3, m_pPlayer->Set_SkillGauge(fRandom(120.f, 150.f)));
	}
	if (m_bHit4)
	{
		IMPL_ONCE(m_bSound[14], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge4, m_pPlayer->Set_SkillGauge(fRandom(120.f, 150.f)));
	}
	if ((m_bHit0 || m_bHit1 || m_bHit2 || m_bHit3 || m_bHit4) && !m_bHitCnt)
	{
		CreateCubeBullet();
		m_bHitCnt = true;
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (45 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& !m_bJump)
	{
		m_fAnimSpeed = 1.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		m_pState_Handler->Get_Player()->Get_Transform()->Jump(7.5, pPlayerNavi);
		m_bJump = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Base_Charge::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_bJump = false;
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Base_Charge::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Cube_Attack_Base_Charge* CState_Player_Cube_Attack_Base_Charge::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Base_Charge* pInstance = new CState_Player_Cube_Attack_Base_Charge(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Base_Charge::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Base_Charge* pInstance = new CState_Player_Cube_Attack_Base_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Base_Charge::Free()
{
}
