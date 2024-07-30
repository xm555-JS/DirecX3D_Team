#include "..\Public\State_Nemesis_Attack3.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Attack3::CState_Nemesis_Attack3(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Attack3::CState_Nemesis_Attack3(const CState_Nemesis_Attack3& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Attack3::Initialize_Prototype()
{
	//// 다음으로 넘어갈 수 있는 state
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_SHIELD);

	//// 다음 state로 넘어갈 확률
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_SHIELD] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Attack3::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Attack3::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Nemesis_Attack3::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Attack3::Exit(void* pArg)
{
}

_bool CState_Nemesis_Attack3::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Attack3* CState_Nemesis_Attack3::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Attack3* pInstance = new CState_Nemesis_Attack3(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Attack3::Clone(void* pArg)
{
	CState_Nemesis_Attack3* pInstance = new CState_Nemesis_Attack3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Attack3::Free()
{
	__super::Free();
}
