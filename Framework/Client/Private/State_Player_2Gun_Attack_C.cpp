#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_C.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_2Gun_Attack_C::CState_Player_2Gun_Attack_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_C::CState_Player_2Gun_Attack_C(const CState_Player_2Gun_Attack_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_D);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_OBB, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_C::Tick(_float fTimeDelta)
{

	

	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 3, 7, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 13, 17, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1, "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 23, 27, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2, "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 30, 34, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3, "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 38, 40, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit4, "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(65.f, 75.f), 47, 53, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit5, "Player_Gun_Hit");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.032f, 0.4f, 0.2f));
	}
	if (m_bHit1)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge1, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		//IMPL_ONCE(m_bShaked1, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 0.12f, 0.06f));
	}
	if (m_bHit2)
	{
		IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge2, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		//IMPL_ONCE(m_bShaked2, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 0.12f, 0.06f));
	}
	if (m_bHit3)
	{
		IMPL_ONCE(m_bSound[13], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge3, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		//IMPL_ONCE(m_bShaked3, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 0.12f, 0.06f));
	}
	if (m_bHit4)
	{
		IMPL_ONCE(m_bSound[14], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge4, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		//IMPL_ONCE(m_bShaked4, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 0.12f, 0.06f));
	}
	if (m_bHit5)
	{
		IMPL_ONCE(m_bSound[15], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge5, m_pPlayer->Set_SkillGauge(fRandom(65.f, 75.f)));
		//IMPL_ONCE(m_bShaked5, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.05f, 0.12f, 0.06f));
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_pColCom->Update(ColMatrix);

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

	//60
	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		40 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;



	if (0 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER,m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_A_5", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(-0.3f, 1.2f, 0.9f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));

	}
		
	if (10 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(-0.21f, 1.4f, 1.f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}
		
	if (20 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect2,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(-0.3f, 1.f, 0.9f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (30 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect3,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_A_5", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.15f, 1.1f, 1.1f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));

	}
		
	if (37 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect4,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(-0.2f, 1.f, 0.9f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));

	}
		
	if (47 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect5,
			_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
		EFFECT_POS_ROT("Player_Gun_Attack_A_5", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.3f, 1.4f, 0.9f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));

	}
		


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_C::Exit(void* pArg)
{

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_C::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_2Gun_Attack_C* CState_Player_2Gun_Attack_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_C* pInstance = new CState_Player_2Gun_Attack_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_C::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_C* pInstance = new CState_Player_2Gun_Attack_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_C::Free()
{
}
