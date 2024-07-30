#include "stdafx.h"
#include "..\Public\State_Robella_Ready.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Ready::CState_Robella_Ready(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Ready::CState_Robella_Ready(const CState_Robella_Ready& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Ready::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_APEAR);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Robella_Ready::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Ready::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_Ready::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//트리거
	if (nullptr != ((CEnemy*)m_pOwner)->Get_Target())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Ready::Exit(void* pArg)
{
}

_bool CState_Robella_Ready::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Ready* CState_Robella_Ready::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Ready* pInstance = new CState_Robella_Ready(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Ready::Clone(void* pArg)
{
	CState_Robella_Ready* pInstance = new CState_Robella_Ready(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Ready::Free()
{
	__super::Free();
}