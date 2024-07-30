#include "stdafx.h"
#include "..\Public\State_Player_Skate_Jump_Landing.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Skate_Jump_Landing::CState_Player_Skate_Jump_Landing(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Skate(pManager, eState)
{
}

CState_Player_Skate_Jump_Landing::CState_Player_Skate_Jump_Landing(const CState_Player_Skate_Jump_Landing& rhs)
	: CState_Player_Skate(rhs)
{
}

HRESULT CState_Player_Skate_Jump_Landing::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKATE_STOP_LAND);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_STOP_WATER);

	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN_SWERVE_L);
	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN_SWERVE_R);
	m_ReserveStates.push_back(STATE_PLAYER_SKATE_RUN);

	return S_OK;
}

HRESULT CState_Player_Skate_Jump_Landing::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Skate_Jump_Landing::Enter(void* pArg)
{
	//204
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
}

void CState_Player_Skate_Jump_Landing::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	Set_PlayerLook(fTimeDelta);

	CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Transform();
	CNavigation* pPlayerNavi = (CNavigation*)m_pPlayer->Get_Component(ID_UINT(NAVIGATION));

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
		pPlayerTransform->Go_AccFront_Stop(fTimeDelta, /*DEBUGFLOAT*/0.01f, m_fForce, pPlayerNavi, &m_fRunSpeed);

	}

	_vector vPos = m_pPlayer->Get_Pos();

	if (10.5f > XMVectorGetY(vPos))
	{
		XMVectorSetY(vPos, 10.5f);
		pPlayerTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Skate_Jump_Landing::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	// 만약 Female일 경우
	//m_pTargetModel->Set_AnimationGroup(m_iCurAnimGroup, m_iCurAnimState);
	__super::Exit();
}

_bool CState_Player_Skate_Jump_Landing::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_SPACE) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Skate_Jump_Landing* CState_Player_Skate_Jump_Landing::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Skate_Jump_Landing* pInstance = new CState_Player_Skate_Jump_Landing(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Skate_Jump_Landing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Skate_Jump_Landing::Clone(void* pArg)
{
	CState_Player_Skate_Jump_Landing* pInstance = new CState_Player_Skate_Jump_Landing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Skate_Jump_Landing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Skate_Jump_Landing::Free()
{
}
