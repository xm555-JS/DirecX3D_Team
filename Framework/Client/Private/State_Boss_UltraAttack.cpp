#include "stdafx.h"
#include "..\Public\State_Boss_UltraAttack.h"
#include "GameInstance.h"

CState_Boss_UltraAttack::CState_Boss_UltraAttack(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Boss_UltraAttack::CState_Boss_UltraAttack(const CState_Boss_UltraAttack& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss_UltraAttack::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_IDLE);

	return S_OK;
}

HRESULT CState_Boss_UltraAttack::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Boss_UltraAttack::Enter(void* pArg)
{
	m_bEndCurState = false;
}

void CState_Boss_UltraAttack::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Boss_UltraAttack::Exit(void* pArg)
{
}

_bool CState_Boss_UltraAttack::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	// 이건 플레이어랑 일정 범위 안에 어느정도 있으면 true를 반환하게 할 예정
	if (KEY_DOWN(VK_NUMPAD1))
		return true;

	return false;
}


CState_Boss_UltraAttack* CState_Boss_UltraAttack::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Boss_UltraAttack* pInstance = new CState_Boss_UltraAttack(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Boss_UltraAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Boss_UltraAttack::Clone(void* pArg)
{
	CState_Boss_UltraAttack* pInstance = new CState_Boss_UltraAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Boss_UltraAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Boss_UltraAttack::Free()
{
}
