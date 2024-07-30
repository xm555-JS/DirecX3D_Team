#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Air_C.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Unit.h"
#include "Player.h"
#include "Navigation.h"

#include "Imgui_Manager.h"

CState_Player_Cube_Attack_Air_C::CState_Player_Cube_Attack_Air_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Cube(pManager, eState)
{
}

CState_Player_Cube_Attack_Air_C::CState_Player_Cube_Attack_Air_C(const CState_Player_Cube_Attack_Air_C& rhs)
	: CState_Player_Cube(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Air_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Air_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Air_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_iSound = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	Use_StaminaGauge(0.1f);
}

void CState_Player_Cube_Attack_Air_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Landing.wav"), CSound_Manager::CHANNEL_PLAYER, m_pPlayer->Get_Pos(), 0.5f));

	if (0 < Get_CurKeyFrame() && 37 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Attack_Charge_M", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		m_fTimeAcc += fTimeDelta;
		if (0.1f < m_fTimeAcc)
		{
			IMPL_ONCE(m_bSound[m_iSound], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_D.wav"), CSound_Manager::CHANNEL_PLAYER, m_pPlayer->Get_Pos(), 0.5f));

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

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200.f, 220.f), 0, 7, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200.f, 220.f), 8, 14, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200.f, 220.f), 15, 21, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200.f, 220.f), 22, 28, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Hit_Cube");
	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200.f, 220.f), 29, 35, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Hit_Cube");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200.f, 220.f)));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(200.f, 220.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.04f, 0.17f, 0.17f));
	}
	if (m_bHit2)
	{
		IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge2, m_pPlayer->Set_SkillGauge(fRandom(200.f, 220.f)));
	}
	if (m_bHit3)
	{
		IMPL_ONCE(m_bSound[13], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge3, m_pPlayer->Set_SkillGauge(fRandom(200.f, 220.f)));
	}
	if (m_bHit4)
	{
		IMPL_ONCE(m_bSound[14], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge4, m_pPlayer->Set_SkillGauge(fRandom(200.f, 220.f)));
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

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
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

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		30 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}
		
	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Air_C::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Air_C::Check_Condition(CState_Handler* pHandler)
{
	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float		 fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 1.f)
		return true;

	return false;
}


CState_Player_Cube_Attack_Air_C* CState_Player_Cube_Attack_Air_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Air_C* pInstance = new CState_Player_Cube_Attack_Air_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Air_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Air_C::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Air_C* pInstance = new CState_Player_Cube_Attack_Air_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Air_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Air_C::Free()
{
}
