#include "stdafx.h"
#include "..\Public\State_Angel_Apear_Ready.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Angel_Apear_Ready::CState_Angel_Apear_Ready(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Apear_Ready::CState_Angel_Apear_Ready(const CState_Angel_Apear_Ready& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Apear_Ready::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_STAND);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_STAND] = 1;

	return S_OK;
}

HRESULT CState_Angel_Apear_Ready::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Apear_Ready::Enter(void* pArg)
{
}

void CState_Angel_Apear_Ready::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 5.f)
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Apear_Ready::Exit(void* pArg)
{
}

_bool CState_Angel_Apear_Ready::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return false;
}

CState_Angel_Apear_Ready* CState_Angel_Apear_Ready::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Apear_Ready* pInstance = new CState_Angel_Apear_Ready(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Apear_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Apear_Ready::Clone(void* pArg)
{
	CState_Angel_Apear_Ready* pInstance = new CState_Angel_Apear_Ready(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Apear_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Apear_Ready::Free()
{
	__super::Free();
}
