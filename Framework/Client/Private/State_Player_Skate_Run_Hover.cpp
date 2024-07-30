#include "stdafx.h"
#include "..\Public\State_Player_Skate_Run_Hover.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Model.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

#include "Imgui_Manager.h"

CState_Player_Skate_Run_Hover::CState_Player_Skate_Run_Hover(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Skate(pManager, eState)
{
}

CState_Player_Skate_Run_Hover::CState_Player_Skate_Run_Hover(const CState_Player_Skate_Run_Hover& rhs)
	: CState_Player_Skate(rhs)
{
}

HRESULT CState_Player_Skate_Run_Hover::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKATE_STOP_LAND);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_STOP_WATER);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);

	//m_ReserveStates.push_back(STATE_PLAYER_SKATE_JUMP);
	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN_SWERVE_L);
	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN_SWERVE_R);
	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN);

	return S_OK;
}

HRESULT CState_Player_Skate_Run_Hover::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skate_Run_Hover::Enter(void* pArg)
{
	//200
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_bEndCurState = true;
	m_fAnimSpeed = 2.f;

	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
}

void CState_Player_Skate_Run_Hover::Tick(_float fTimeDelta)
{
	
	USEGAMEINSTANCE;

	Set_PlayerLook(fTimeDelta);

	m_fTimeAcc_Effect2 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect2)
	{
		m_fTimeAcc_Effect2 = 0.f;
		EFFECT_POS("Skate_Boat_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
	}

	if (KEY_DOWN('W') ||
		KEY_DOWN('A') ||
		KEY_DOWN('S') ||
		KEY_DOWN('D'))
	{
		GAMEINSTANCE->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.f, 1.f, 1.f, 0.018f, false, true);
	}
	else if(KEY_UP('W') ||
			KEY_UP('A') ||
			KEY_UP('S') ||
			KEY_UP('D'))
	{
		GAMEINSTANCE->Stop_RadialBlur();
	}

	_vector vPos = m_pPlayer->Get_Pos();

	if (10.6f > XMVectorGetY(vPos))
	{
		m_fTimeAcc_Effect0 += fTimeDelta;
		m_fTimeAcc_Effect1 += fTimeDelta;
	}

	if (0.2f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_EXCUTE("Player_Swim_Dash2",
			XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixTranslation(0.1f, 0.f, 1.3f),
			1.5f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL);
	}
	if (0.015f < m_fTimeAcc_Effect1)
	{
		m_fTimeAcc_Effect1 = 0.f;

		// Left		
		_matrix matFinalWorld = 
			XMMatrixScaling(3.f, 3.f, 3.f) * 
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) * 
			XMMatrixTranslation(fRandom(-0.1f, 0.1f) + 0.3f, fRandom(-0.05f, 0.05f) + 0.f, fRandom(-0.05f, 0.05f) + 0.7f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld * m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix(), 1.6f);

		matFinalWorld =
			XMMatrixScaling(3.f, 3.f, 3.f) * 
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) * 
			XMMatrixTranslation(fRandom(-0.1f, 0.1f) + 0.3f, fRandom(-0.05f, 0.05f) + 0.f, fRandom(-0.05f, 0.05f) + 0.7f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld * m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix(), 1.6f);
		
		// right
		matFinalWorld =
			XMMatrixScaling(3.f, 3.f, 3.f) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) *
			XMMatrixTranslation(fRandom(-0.1f, 0.1f) - 0.3f, fRandom(-0.05f, 0.05f) + 0.f, fRandom(-0.05f, 0.05f) + 0.7f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld * m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix(), 1.6f);
		
		matFinalWorld =
			XMMatrixScaling(3.f, 3.f, 3.f) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) *
			XMMatrixTranslation(fRandom(-0.1f, 0.1f) - 0.3f, fRandom(-0.05f, 0.05f) + 0.f, fRandom(-0.05f, 0.05f) + 0.7f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld * m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix(), 1.6f);

		// Middle Back
		matFinalWorld =
			XMMatrixScaling(1.2f, 1.2f, 1.2f) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) *
			XMMatrixTranslation(fRandom(-0.1f, 0.1f), fRandom(-0.05f, 0.05f) + 0.f, fRandom(-0.05f, 0.05f) + 0.3F);
		EFFECT_EXCUTE("Player_Swim_Loop2_SoftX", matFinalWorld * m_pState_Handler->Get_Player()->Get_Transform()->Get_WorldMatrix(), 1.5f);
	}

	CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Transform();
	CNavigation* pPlayerNavi = (CNavigation*)m_pPlayer->Get_Component(ID_UINT(NAVIGATION));

	if (10.6f > XMVectorGetY(vPos))
	{
		if (KEY_PRESSING('W') ||
			KEY_PRESSING('A') ||
			KEY_PRESSING('S') ||
			KEY_PRESSING('D'))
		{
			m_pPlayer->Get_Transform()->Reset_StopAccTime();
			_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
			_float	fRunSpeed = fPlayerSpeed * 12.f;

			pPlayerTransform->Go_AccFront_Limit(fTimeDelta, fRunSpeed, 16.f, pPlayerNavi, &m_fRunSpeed);
		}
		else
		{
			m_pPlayer->Get_Transform()->Reset_FrontAccTime();
			pPlayerTransform->Go_AccFront_Stop(fTimeDelta, 5.f, m_fForce, pPlayerNavi, &m_fRunSpeed);
		}
	}
	else
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 12.f;

		pPlayerTransform->Go_AccFront_Stop(fTimeDelta, 12.f, m_fForce, pPlayerNavi, &m_fRunSpeed);

		if(0.f >= m_fRunSpeed)
			m_pPlayer->Get_Transform()->Reset_StopAccTime();
	}

	CState_Player_Skate::Tick(fTimeDelta);
}

void CState_Player_Skate_Run_Hover::Exit(void* pArg)
{ 
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Skate_Run_Hover::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}


CState_Player_Skate_Run_Hover* CState_Player_Skate_Run_Hover::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skate_Run_Hover* pInstance = new CState_Player_Skate_Run_Hover(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skate_Run_Hover");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skate_Run_Hover::Clone(void* pArg)
{
	CState_Player_Skate_Run_Hover* pInstance = new CState_Player_Skate_Run_Hover(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skate_Run_Hover");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skate_Run_Hover::Free()
{
}
