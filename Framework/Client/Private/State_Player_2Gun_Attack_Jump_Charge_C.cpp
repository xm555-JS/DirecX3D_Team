#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Jump_Charge_C.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Navigation.h"


CState_Player_2Gun_Attack_Jump_Charge_C::CState_Player_2Gun_Attack_Jump_Charge_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Jump_Charge_C::CState_Player_2Gun_Attack_Jump_Charge_C(const CState_Player_2Gun_Attack_Jump_Charge_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_C::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_2GUN_SKILL_NORMAL);
	//m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	//m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_B);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Jump_Charge_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 5.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_Jump_Charge_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (5 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_Jump_Charge_C.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		m_fAnimSpeed = 2.5f;
		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Gun_Attack_Air_C", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
	}

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(300.f,350.f), 0, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0);

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking({ -1.f,0.f,0.f,0.f }, 0.046f, 0.18f, 0.18f));
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);
	m_pColCom->Update(ColMatrix);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	/*if (KEY_PRESSING('W') ||
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

	if (m_pState_Handler->Get_KeyInput() &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}*/

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;



	CState_Player::Tick(fTimeDelta);
}
void CState_Player_2Gun_Attack_Jump_Charge_C::Exit(void* pArg)
{
	//m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	//m_pColCom->Set_ColliderState(false);

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Jump_Charge_C::Check_Condition(CState_Handler* pHandler)
{
	//USEGAMEINSTANCE;
	//if (KEY_DOWN(VK_LBUTTON))
	//{
	//	return true;
	//}

	//return false;
	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float		 fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 1.f)
		return true;

	return false;
}


CState_Player_2Gun_Attack_Jump_Charge_C* CState_Player_2Gun_Attack_Jump_Charge_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Jump_Charge_C* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Jump_Charge_C::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Jump_Charge_C* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Jump_Charge_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Jump_Charge_C::Free()
{
}
