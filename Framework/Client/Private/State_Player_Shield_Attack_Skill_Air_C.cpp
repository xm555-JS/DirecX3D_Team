#include "stdafx.h"
#include "..\Public\State_Player_Shield_Attack_Skill_Air_C.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"
#include "Player.h"

CState_Player_Shield_Attack_Skill_Air_C::CState_Player_Shield_Attack_Skill_Air_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Attack_Skill_Air_C::CState_Player_Shield_Attack_Skill_Air_C(const CState_Player_Shield_Attack_Skill_Air_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Attack_Skill_Air_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Shield_Attack_Skill_Air_C::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Shield_Attack_Skill_Air_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Shield_Attack_Skill_Air_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (5 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Axe_Hit_Ground.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Player_Axe_Attack_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 1.5f, 1.f), // Pos
				XMVectorSet(0.f, XMConvertToRadians(80), 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(1.2f, 1.2f, 1.2f, 1.f) // Scale		
			)
		);
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.07f, 0.34f, 0.34f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.008f, 0.f, 0.004f, 0.f), 0.4f, 0.4f);
		);

	}

	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, fRandom(1100.f, 1250.f), 0, 30, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_ShieldAxe");

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.5f);

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

	if (m_pState_Handler->Get_KeyInput() &&
		30 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Attack_Skill_Air_C::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Shield_Attack_Skill_Air_C::Check_Condition(CState_Handler* pHandler)
{
	CTransform* pPlayerTransform = (CTransform*)pHandler->Get_Player()->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pHandler->Get_Player()->Get_Component(NAVIGATION);
	_float		 fHightDiff = 0.f;
	pPlayerNavi->isUpperThanPlane(pPlayerTransform, &fHightDiff);

	if (fHightDiff < 1.f)
		return true;

	return false;
}


CState_Player_Shield_Attack_Skill_Air_C* CState_Player_Shield_Attack_Skill_Air_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Attack_Skill_Air_C* pInstance = new CState_Player_Shield_Attack_Skill_Air_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Attack_Skill_Air_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Attack_Skill_Air_C::Clone(void* pArg)
{
	CState_Player_Shield_Attack_Skill_Air_C* pInstance = new CState_Player_Shield_Attack_Skill_Air_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Attack_Skill_Air_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Attack_Skill_Air_C::Free()
{
}
