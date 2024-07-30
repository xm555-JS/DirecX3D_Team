#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_Base_Charge.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Axe_Attack_Base_Charge::CState_Player_Axe_Attack_Base_Charge(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_Base_Charge::CState_Player_Axe_Attack_Base_Charge(const CState_Player_Axe_Attack_Base_Charge& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_Base_Charge::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_AXE_ATTACK_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_Base_Charge::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_Base_Charge::Enter(void* pArg)
{
	__super::Enter(pArg);
	USEGAMEINSTANCE;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_OBB, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Axe_Attack_Base_Charge::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 1.5f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, fRandom(700.f, 750.f), 25, 35, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("AxeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(700.f, 750.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.07f, 0.17f, 0.17f));
	}

	if(25 < Get_CurKeyFrame())	
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Axe_Swing.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));


	if (30 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Axe_Hit_Ground.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Axe_Attack_Charge_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 1.4f, 1.f) // Pos
				//XMVectorSet(0.f, 0.f, 0.f) // Rot
				//1.f,	// PlaySpeed 
				//XMVectorSet(DEBUGFLOAT4(2).x, DEBUGFLOAT4(2).y, DEBUGFLOAT4(2).z, 1.f) // Scale		
			)
		);
		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Player_Axe_Attack_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 2.1f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(80), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}
	if (31 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Player_Axe_Attack_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0, 3.f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}
	if (32 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect3,
			EFFECT_POS("Player_Axe_Attack_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0, 4.f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}
	if (33 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect4,
			EFFECT_POS("Player_Axe_Attack_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0, 5.f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}
	if (34 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect5,
			EFFECT_POS("Player_Axe_Attack_Charge_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0, 6.f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
	}

	/*if (m_bHit0)
	{
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(80.f, 100.f)));
	}*/

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

	if (m_pState_Handler->Get_KeyInput() &&
		50 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_Base_Charge::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_Base_Charge::Check_Condition(CState_Handler* pHandler)
{
	if(0.5f < pHandler->Get_CurState()->Get_KeyPressTimeAcc())
		return true;

	return false;
}


CState_Player_Axe_Attack_Base_Charge* CState_Player_Axe_Attack_Base_Charge::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_Base_Charge* pInstance = new CState_Player_Axe_Attack_Base_Charge(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_Base_Charge::Clone(void* pArg)
{
	CState_Player_Axe_Attack_Base_Charge* pInstance = new CState_Player_Axe_Attack_Base_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_Base_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_Base_Charge::Free()
{
}
