#include "stdafx.h"
#include "..\Public\State_Player_Shield_Attack_Base_Charge_B.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"
#include "Player.h"

CState_Player_Shield_Attack_Base_Charge_B::CState_Player_Shield_Attack_Base_Charge_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Attack_Base_Charge_B::CState_Player_Shield_Attack_Base_Charge_B(const CState_Player_Shield_Attack_Base_Charge_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Attack_Base_Charge_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SHIELD_ATTACK_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_END);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);

	return S_OK;
}

HRESULT CState_Player_Shield_Attack_Base_Charge_B::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Shield_Attack_Base_Charge_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pPlayer->Set_Shield(0.1f);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
	EFFECT_EXCUTE("Player_Shield_Attack_Charge_B1", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");

	IMPL_ONCE(m_bSound[0], m_eSoundChannel[2] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Skill_Charge_B_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));


}

void CState_Player_Shield_Attack_Base_Charge_B::Tick(_float fTimeDelta)
{

	//IMPL_ALARM(m_fSound[1], m_eSoundChannel[1] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Skill_Charge_B_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f),0.2f,fTimeDelta);

	USEGAMEINSTANCE;
	m_fTimeAcc_Effect += fTimeDelta;
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Shield_Skill_Charge_B_3.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.3f, fTimeDelta);

	if (0.7f < m_fTimeAcc_Effect)
	{

		EFFECT_EXCUTE("Player_Shield_Attack_Charge_B1", /*XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix())*/XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_r_wp");
		m_fTimeAcc_Effect = 0.f;
	}
#if 0
	if (DEBUGFLOAT < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Shield_Attack_Charge_A_1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, DEBUGFLOAT4(0).x, 1.f), // Pos
				XMVectorSet(0.f, 0.f, 0.f, 0.f), // Rot
				1.f,	// PlaySpeed 
				XMVectorSet(DEBUGFLOAT4(0).x, DEBUGFLOAT4(0).y, DEBUGFLOAT4(0).z, 1.f) // Scale		
			)
		);
	}
#endif // 0


	if (m_pState_Handler->Get_KeyInput())
	{
		m_bEndCurState = true;
	}
	else if (1.5f < m_pState_Handler->Get_TimeAcc())
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

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Attack_Base_Charge_B::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	//MGR(CSound_Manager)->StopSound(m_eSoundChannel[1]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_pPlayer->Set_Shield(1.f);
	__super::Exit();
}

_bool CState_Player_Shield_Attack_Base_Charge_B::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Shield_Attack_Base_Charge_B* CState_Player_Shield_Attack_Base_Charge_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Attack_Base_Charge_B* pInstance = new CState_Player_Shield_Attack_Base_Charge_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Attack_Base_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Attack_Base_Charge_B::Clone(void* pArg)
{
	CState_Player_Shield_Attack_Base_Charge_B* pInstance = new CState_Player_Shield_Attack_Base_Charge_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Attack_Base_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Attack_Base_Charge_B::Free()
{
}
