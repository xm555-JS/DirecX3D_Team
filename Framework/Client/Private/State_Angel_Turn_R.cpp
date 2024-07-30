#include "stdafx.h"
#include "..\Public\State_Angel_Turn_R.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"

#include "Unit.h"

CState_Angel_Turn_R::CState_Angel_Turn_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Turn_R::CState_Angel_Turn_R(const CState_Angel_Turn_R& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Turn_R::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_RUN);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_RUN] = 1;

	return S_OK;
}

HRESULT CState_Angel_Turn_R::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Turn_R::Enter(void* pArg)
{
}

void CState_Angel_Turn_R::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (23 < m_pTargetModel->Get_Animation()[15]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Turn_R::Exit(void* pArg)
{
	m_pTargetModel->Get_Owner()->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
}

_bool CState_Angel_Turn_R::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Angel_Turn_R* CState_Angel_Turn_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Turn_R* pInstance = new CState_Angel_Turn_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Turn_R::Clone(void* pArg)
{
	CState_Angel_Turn_R* pInstance = new CState_Angel_Turn_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Turn_R::Free()
{
	__super::Free();
}
