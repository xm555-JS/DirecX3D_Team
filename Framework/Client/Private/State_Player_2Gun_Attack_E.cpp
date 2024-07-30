#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_E.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_2Gun_Attack_E::CState_Player_2Gun_Attack_E(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_E::CState_Player_2Gun_Attack_E(const CState_Player_2Gun_Attack_E& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_E::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	//m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_E::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_E::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_OBB, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());


}

void CState_Player_2Gun_Attack_E::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(220.f,270.f), 55, 80, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Gun_Hit");

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += 3.f * XMVector3Normalize(ColMatrix.r[2]);
	_float4 vColScale = m_vColScale;
	vColScale.z *= 2.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&vColScale));
	m_pColCom->Update(ColMatrix);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	//65
	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		65 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking({ -1.f,0.f,0.f,0.f }, 0.025f, 0.4f, 0.2f));
	}


	IMPL_ONCE(m_bEffect0,
		EFFECT_EXCUTE("Player_Gun_Converge_Black", XMMatrixIdentity(), 1.05f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bip001-R-Finger0Nub"));
	
	IMPL_ONCE(m_bEffect1,
		EFFECT_EXCUTE("Player_Gun_Converge_P", XMMatrixIdentity(), 1.05f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bip001-R-Finger0Nub"));
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_E_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	if (50 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_E_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Player_Gun_Attack_D", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.15f, 1.1f, 0.8f, 1.f), XMVectorSet(1.571f, 0.f, 0.f, 0.f), 0.8f, XMVectorSet(3.f, 3.f, 4.f, 0.f)));
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_E::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_E::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_2Gun_Attack_E* CState_Player_2Gun_Attack_E::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_E* pInstance = new CState_Player_2Gun_Attack_E(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_E::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_E* pInstance = new CState_Player_2Gun_Attack_E(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_E::Free()
{
}
