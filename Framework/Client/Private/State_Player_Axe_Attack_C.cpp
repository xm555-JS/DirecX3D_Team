#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_C.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Unit.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Axe_Attack_C::CState_Player_Axe_Attack_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_C::CState_Player_Axe_Attack_C(const CState_Player_Axe_Attack_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_AXE_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_AXE_ATTACK_D);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Axe_Attack_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]) * 0.5f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, fRandom(300.f, 360.f), 13, 25, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("AxeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(300.f, 360.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.04f, 0.17f, 0.17f));
	}


	if (13 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Axe_Attack_3.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Axe_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.9f, 1.f, 0.6f, 1.f), // Pos
				XMVectorSet(XMConvertToRadians(-90.f), XMConvertToRadians(80.f), 0.f, 0.f) // Rot	
			)
		);
	}
	if (22 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Player_Axe_Attack_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(-0.3f, 0, 2.1f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(80), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}

	//IMPL_ONCE(m_bHit, ));
//	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(200, 220), 7, 15, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Sword_Hit_Degree45");

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


	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_C::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_C::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Axe_Attack_C* CState_Player_Axe_Attack_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_C* pInstance = new CState_Player_Axe_Attack_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_C::Clone(void* pArg)
{
	CState_Player_Axe_Attack_C* pInstance = new CState_Player_Axe_Attack_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_C::Free()
{
}
