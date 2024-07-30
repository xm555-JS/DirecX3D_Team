#include "stdafx.h"
#include "..\Public\State_Player_Shield_Attack_Jump_Charge_B.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Unit.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Shield_Attack_Jump_Charge_B::CState_Player_Shield_Attack_Jump_Charge_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Attack_Jump_Charge_B::CState_Player_Shield_Attack_Jump_Charge_B(const CState_Player_Shield_Attack_Jump_Charge_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Attack_Jump_Charge_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_C);

	return S_OK;
}

HRESULT CState_Player_Shield_Attack_Jump_Charge_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Shield_Attack_Jump_Charge_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAccTime = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Shield_Attack_Jump_Charge_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Jump_Charge_A.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Jump_Charge_B.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);

	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, fRandom(50.f, 75.f), 0, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("ShieldHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(50.f, 75.f)));
	}

	if (20 < Get_CurKeyFrame())
	{
		m_bHit0 = false;
		RESET_ONCE(m_bGauge0);
	}

	if (0 <= Get_CurKeyFrame())
	{
		if ((Get_CurKeyFrame() % 3))
		{
			IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Shield_Attack_Jump_Charge_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
			RESET_ONCE(m_bEffect0);
		}
	}

	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Hit_ShieldAxe");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = 15.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	//CTransform* pCurCameraTransform = pGameInstance->Get_CurrentCamera()->Get_Transform();
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();
	//s = v0 * t + 0.5 * a * t^2

	pPlayerTransform->Go_AccFront(fTimeDelta, fRunSpeed, pPlayerNavi);

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Attack_Jump_Charge_B::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_pState_Handler->Get_Player()->Get_Transform()->Reset_FrontAccTime();

	__super::Exit();
}

_bool CState_Player_Shield_Attack_Jump_Charge_B::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Shield_Attack_Jump_Charge_B* CState_Player_Shield_Attack_Jump_Charge_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Attack_Jump_Charge_B* pInstance = new CState_Player_Shield_Attack_Jump_Charge_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Attack_Jump_Charge_B::Clone(void* pArg)
{
	CState_Player_Shield_Attack_Jump_Charge_B* pInstance = new CState_Player_Shield_Attack_Jump_Charge_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Attack_Jump_Charge_B::Free()
{
}
