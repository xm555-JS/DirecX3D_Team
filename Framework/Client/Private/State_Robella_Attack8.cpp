#include "stdafx.h"
#include "..\Public\State_Robella_Attack8.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Attack8::CState_Robella_Attack8(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack8::CState_Robella_Attack8(const CState_Robella_Attack8& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack8::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack8::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack8::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_Attack8::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	//일단 안씀

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack8::Exit(void* pArg)
{
}

_bool CState_Robella_Attack8::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack8* CState_Robella_Attack8::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack8* pInstance = new CState_Robella_Attack8(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack8");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack8::Clone(void* pArg)
{
	CState_Robella_Attack8* pInstance = new CState_Robella_Attack8(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack8");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack8::Free()
{
	__super::Free();
}
