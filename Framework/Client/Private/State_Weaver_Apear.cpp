#include "stdafx.h"
#include "..\Public\State_Weaver_Apear.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Apear::CState_Weaver_Apear(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Apear::CState_Weaver_Apear(const CState_Weaver_Apear& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Apear::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_CAM_ACTION);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_CAM_ACTION] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Apear::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 1.f;
	GAMEINSTANCE->Play_ActionCamera(TEXT("Weaver_CameraAction"), 1.f);		
}

void CState_Weaver_Apear::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	//	m_bEndCurState = true;

	m_fAccTime += fTimeDelta;
	if (1.f <= m_fAccTime)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Apear::Exit(void* pArg)
{
}

_bool CState_Weaver_Apear::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Weaver_Apear* CState_Weaver_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Apear* pInstance = new CState_Weaver_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Apear::Clone(void* pArg)
{
	CState_Weaver_Apear* pInstance = new CState_Weaver_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Apear::Free()
{
	__super::Free();
}
