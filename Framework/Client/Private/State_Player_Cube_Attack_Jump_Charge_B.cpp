#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Jump_Charge_B.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"
#include "Bullet_Player_Cube.h"

CState_Player_Cube_Attack_Jump_Charge_B::CState_Player_Cube_Attack_Jump_Charge_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Jump_Charge_B::CState_Player_Cube_Attack_Jump_Charge_B(const CState_Player_Cube_Attack_Jump_Charge_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_END);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Jump_Charge_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Jump_Charge_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	m_fUseStaminaTime = 0.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());


	list<CGameObject*>* CubeBullets = (GAMEINSTANCE->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));
	if (nullptr == CubeBullets || 0 == CubeBullets->size())
		return;

	for (auto& iter : *CubeBullets)
	{
		((CBullet_Player_Cube*)iter)->Set_Shoot();
	}
}

void CState_Player_Cube_Attack_Jump_Charge_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fUseStaminaTime += fTimeDelta;

	if (0.5f < m_fUseStaminaTime)
	{
		Use_StaminaGauge(0.1f);
		m_fUseStaminaTime = 0.f;
	}

	if(2.f < m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Jump_Charge_B::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Jump_Charge_B::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Cube_Attack_Jump_Charge_B* CState_Player_Cube_Attack_Jump_Charge_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Jump_Charge_B* pInstance = new CState_Player_Cube_Attack_Jump_Charge_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Jump_Charge_B::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Jump_Charge_B* pInstance = new CState_Player_Cube_Attack_Jump_Charge_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Jump_Charge_B::Free()
{
}
