#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End.h"
#include "GameInstance.h"
#include "Enemy.h"

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Tick(_float fTimeDelta)
{

	if (!Is_AnimFinished())
	{
		if (2 < Get_CurKeyFrame())
			IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Double_Dead.wav"), CSound_Manager::CHANNEL_ENEMY,m_pOwner->Get_Pos(), 0.1f));

		CState_AI::Tick(fTimeDelta);
	}
	else
	{
		((CEnemy*)m_pOwner)->Set_IsDissolve(true);
		m_fAccTime += fTimeDelta;

		if (m_fAccTime > 3.f)
		{
			GAMEINSTANCE->Add_DeadObject(m_pOwner);
			return;
		}
	}
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Exit(void* pArg)
{

}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Check_Condition(CState_Handler* pHandler)
{
	return Is_OwnerDead(pHandler);
}


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Free()
{
	__super::Free();
}
