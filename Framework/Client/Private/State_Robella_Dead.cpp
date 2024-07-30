#include "stdafx.h"
#include "..\Public\State_Robella_Dead.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Dead::CState_Robella_Dead(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Dead::CState_Robella_Dead(const CState_Robella_Dead& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Dead::Initialize_Prototype()
{
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE);
	//
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Robella_Dead::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Dead::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_Dead::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (10 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Dead.wav"),
			CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 1.f));
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		((CEnemy*)m_pOwner)->Set_IsDissolve(true);
		m_fAccTime += fTimeDelta;
	}
	else
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);

	if (m_fAccTime > 3.f)
	{
		GAMEINSTANCE->Add_DeadObject(m_pOwner);
		return;
	}
}

void CState_Robella_Dead::Exit(void* pArg)
{
}

_bool CState_Robella_Dead::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	if (0 >= ((CUnit*)pHandler->Get_Owner())->Get_UnitCurInfo()->fHP)
		return true;

	return false;
}

CState_Robella_Dead* CState_Robella_Dead::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Dead* pInstance = new CState_Robella_Dead(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Dead::Clone(void* pArg)
{
	CState_Robella_Dead* pInstance = new CState_Robella_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Dead::Free()
{
	__super::Free();
}
