#include "stdafx.h"
#include "..\Public\State_Player_Sword_Attack_Air_B.h"
#include "GameInstance.h"
#include "Channel.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_Sword_Attack_Air_B::CState_Player_Sword_Attack_Air_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Attack_Air_B::CState_Player_Sword_Attack_Air_B(const CState_Player_Sword_Attack_Air_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Attack_Air_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_A);
	m_ReserveStates.push_back(STATE_PLAYER_SWORD_ATTACK_AIR_C);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);


	return S_OK;
}

HRESULT CState_Player_Sword_Attack_Air_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Sword_Attack_Air_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	
	m_fAnimSpeed = 2.f;
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
	Use_StaminaGauge(0.1f);
}

void CState_Player_Sword_Attack_Air_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//if (13 <= Get_CurKeyFrame())
	//	IMPL_ONCE(m_bEffect0, EFFECT_POS("Attack_Jump_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.5f));

	if (13 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordAttack_Air_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 2.1f, 0.f, 1.f),
				XMVectorSet(XMConvertToRadians(200), XMConvertToRadians(90), 0.f, 0.f),
				1.5f
			)
		);
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]) * 0.5f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 5.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(200.f, 250.f), 13, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree-45");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SwordHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200.f, 250.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.028f, 0.2f, 0.2f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
		(KEY_DOWN(VK_RBUTTON)) ||
		(KEY_DOWN(VK_SHIFT)) ||
		(KEY_DOWN(VK_SPACE)))
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


	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		20 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
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

void CState_Player_Sword_Attack_Air_B::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Attack_Air_B::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey && Enable_Use_Stamina(pHandler))
			return true;
	}

	return false;
}


CState_Player_Sword_Attack_Air_B* CState_Player_Sword_Attack_Air_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Attack_Air_B* pInstance = new CState_Player_Sword_Attack_Air_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Attack_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Attack_Air_B::Clone(void* pArg)
{
	CState_Player_Sword_Attack_Air_B* pInstance = new CState_Player_Sword_Attack_Air_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Attack_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Attack_Air_B::Free()
{
}
