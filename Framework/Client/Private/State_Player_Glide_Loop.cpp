#include "stdafx.h"
#include "..\Public\State_Player_Glide_Loop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"
#include "Effect_Base.h"

CState_Player_Glide_Loop::CState_Player_Glide_Loop(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Glide_Loop::CState_Player_Glide_Loop(const CState_Player_Glide_Loop& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Glide_Loop::Initialize_Prototype()
{
	//m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_FALL);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_BEGIN_F);			//STATE_PLAYER_GLIDE_BEGIN_F,						//154
	m_ReserveStates.push_back(STATE_PLAYER_GLIDE_END);				//STATE_PLAYER_GLIDE_END,							//153

	return S_OK;
}

HRESULT CState_Player_Glide_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Glide_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;

	USEGAMEINSTANCE;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Glide(0.f, pPlayerNavi);
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		m_pPlayer->Get_CurWeaponL()->Set_IsDissolve(true);
		m_pPlayer->Get_CurWeaponL()->Set_IsUsing(false);
		m_pPlayer->Get_CurWeaponL()->Set_IsGlow(true);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		m_pPlayer->Get_CurWeaponR()->Set_IsDissolve(true);
		m_pPlayer->Get_CurWeaponR()->Set_IsUsing(false);
		m_pPlayer->Get_CurWeaponR()->Set_IsGlow(true);
	}

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);

	m_listEffect.clear();
	EFFECT_EXCUTE("Glider_M1",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),	
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);
	EFFECT_EXCUTE("Glider_M1",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f)) * XMMatrixTranslation(-0.38f, 0.f, -0.1f) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f)),
		1.f,
		m_pState_Handler->Get_Player(),
		CEffect_Manager::FOLLOW_POS_RUL,
		"Bone_wp", &m_listEffect);

	//GAMEINSTANCE->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.f, 1.f, 1.f, 0.017f, false, true);
}

void CState_Player_Glide_Loop::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Base.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 0.8f, fTimeDelta);

	//IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Move.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
	//IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Glider_Move.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f), 4.f, fTimeDelta);

	Set_PlayerLook(fTimeDelta);

	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.1f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("Dash_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}
	//공중에 떠있음//조금씩 떨어짐

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Glide_Loop::Exit(void* pArg)
{
	for (auto& iter : m_listEffect)
		iter->Set_EffectOff();

	m_fKeyPressTimeAcc = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	__super::Exit();
}

_bool CState_Player_Glide_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if(STATE_PLAYER_GLIDE_UP_END == pHandler->Get_CurStateType() ||
		STATE_PLAYER_GLIDE_LOOP_F == pHandler->Get_CurStateType() ||
		STATE_PLAYER_GLIDE_BEGIN_F == pHandler->Get_CurStateType() )
	{
		if (KEY_NONE('W') &&
			KEY_NONE('A') &&
			KEY_NONE('S') &&
			KEY_NONE('D'))
		{
			return true;
		}
	}
	else if(pHandler->Get_Player()->Get_Transform()->Is_InAir())
	{
		if (KEY_DOWN('2'))
		{
			return true;
		}
	}


	return false;
}


CState_Player_Glide_Loop* CState_Player_Glide_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Glide_Loop* pInstance = new CState_Player_Glide_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Glide_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Glide_Loop::Clone(void* pArg)
{
	CState_Player_Glide_Loop* pInstance = new CState_Player_Glide_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Glide_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Glide_Loop::Free()
{
}
