#include "stdafx.h"
#include "..\Public\State_Angel_BackWard.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"
#include "Unit.h"

CState_Angel_BackWard::CState_Angel_BackWard(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_BackWard::CState_Angel_BackWard(const CState_Angel_BackWard& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_BackWard::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Angel_BackWard::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_BackWard::Enter(void* pArg)
{
}

void CState_Angel_BackWard::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	_float	fPlayerSpeed = ((CUnit*)m_pTargetModel->Get_Owner())->Get_UnitInfo()->fSpeed;

	((CUnit*)m_pTargetModel->Get_Owner())->Get_Transform()->Go_Backward(fTimeDelta, fPlayerSpeed);

	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_BackWard::Exit(void* pArg)
{
}

_bool CState_Angel_BackWard::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Angel_BackWard* CState_Angel_BackWard::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_BackWard* pInstance = new CState_Angel_BackWard(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_BackWard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_BackWard::Clone(void* pArg)
{
	CState_Angel_BackWard* pInstance = new CState_Angel_BackWard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_BackWard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_BackWard::Free()
{
	__super::Free();
}
