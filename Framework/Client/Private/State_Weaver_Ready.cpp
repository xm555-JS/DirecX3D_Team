#include "stdafx.h"
#include "..\Public\State_Weaver_Ready.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Ready::CState_Weaver_Ready(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Ready::CState_Weaver_Ready(const CState_Weaver_Ready& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Ready::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_APEAR);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Ready::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Ready::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Ready::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//임시
	m_fAccTime += fTimeDelta;
	//트리거
	if (nullptr != ((CEnemy*)m_pOwner)->Get_Target())
	{
		((CEnemy*)m_pOwner)->Get_Target()->Get_Transform()->YFixedLookAt(m_pState_Handler->Get_Owner()->Get_Pos());
		m_bEndCurState = true;
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Ready::Exit(void* pArg)
{
}

_bool CState_Weaver_Ready::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Weaver_Ready* CState_Weaver_Ready::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Ready* pInstance = new CState_Weaver_Ready(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Ready::Clone(void* pArg)
{
	CState_Weaver_Ready* pInstance = new CState_Weaver_Ready(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Ready::Free()
{
	__super::Free();
}
