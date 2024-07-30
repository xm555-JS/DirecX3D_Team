#include "stdafx.h"
#include "..\Public\State_Player_Axe_Attack_Jump_Charge_C.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Axe_Attack_Jump_Charge_C::CState_Player_Axe_Attack_Jump_Charge_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Axe_Attack_Jump_Charge_C::CState_Player_Axe_Attack_Jump_Charge_C(const CState_Player_Axe_Attack_Jump_Charge_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Axe_Attack_Jump_Charge_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_A);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);

	return S_OK;
}

HRESULT CState_Player_Axe_Attack_Jump_Charge_C::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Axe_Attack_Jump_Charge_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Axe_Attack_Jump_Charge_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Hit_ShieldAxe");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 5.f);

	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, fRandom(600.f, 650.f), 10, 25, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("AxeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(600.f, 650.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.07f, 0.17f, 0.17f));
	}

	if (10 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Axe_Hit_Ground.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Axe_Attack_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 1.5f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(80), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
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

	if (m_pState_Handler->Get_KeyInput() &&
		35 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Axe_Attack_Jump_Charge_C::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Axe_Attack_Jump_Charge_C::Check_Condition(CState_Handler* pHandler)
{
	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float		 fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 1.f)
		return true;

	return false;
}


CState_Player_Axe_Attack_Jump_Charge_C* CState_Player_Axe_Attack_Jump_Charge_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Axe_Attack_Jump_Charge_C* pInstance = new CState_Player_Axe_Attack_Jump_Charge_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Axe_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Axe_Attack_Jump_Charge_C::Clone(void* pArg)
{
	CState_Player_Axe_Attack_Jump_Charge_C* pInstance = new CState_Player_Axe_Attack_Jump_Charge_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Axe_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Axe_Attack_Jump_Charge_C::Free()
{
}
