#include "stdafx.h"
#include "..\Public\State_Player_Shield_Attack_Roll.h"
#include "GameInstance.h"
#include "Channel.h"
//#include "Imgui_Manager.h"
#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_Shield_Attack_Roll::CState_Player_Shield_Attack_Roll(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Attack_Roll::CState_Player_Shield_Attack_Roll(const CState_Player_Shield_Attack_Roll& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Attack_Roll::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SHIELD_ATTACK_SKILL_NORMAL);
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

HRESULT CState_Player_Shield_Attack_Roll::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Shield_Attack_Roll::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Attack_Roll.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

}

void CState_Player_Shield_Attack_Roll::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (55 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Attack_Roll::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Shield_Attack_Roll::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (STATE_PLAYER_ROLL_FORWORD == pHandler->Get_CurStateType() &&
		KEY_DOWN(VK_LBUTTON))
		return true;

	return false;
}


CState_Player_Shield_Attack_Roll* CState_Player_Shield_Attack_Roll::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Attack_Roll* pInstance = new CState_Player_Shield_Attack_Roll(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Attack_Roll::Clone(void* pArg)
{
	CState_Player_Shield_Attack_Roll* pInstance = new CState_Player_Shield_Attack_Roll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Attack_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Attack_Roll::Free()
{
}
