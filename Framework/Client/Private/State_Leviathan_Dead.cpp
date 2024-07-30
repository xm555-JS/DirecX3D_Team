#include "stdafx.h"
#include "..\Public\State_Leviathan_Dead.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Dead::CState_Leviathan_Dead(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Dead::CState_Leviathan_Dead(const CState_Leviathan_Dead& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Dead::Initialize_Prototype()
{
	//m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	//m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Dead::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Dead::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 0.8f;
}

void CState_Leviathan_Dead::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (10 < Get_CurKeyFrame())
	{
		//IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Dead.wav"),
		//	CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 1.f));
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
		pGameInstance->Add_DeadObject(m_pOwner);
		return;
	}

}

void CState_Leviathan_Dead::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Dead::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	list<CGameObject*>* pGameObjs = pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL));

	if (0 == pGameObjs->size())
		return true;


	return false;
}

CState_Leviathan_Dead* CState_Leviathan_Dead::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Dead* pInstance = new CState_Leviathan_Dead(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Dead::Clone(void* pArg)
{
	CState_Leviathan_Dead* pInstance = new CState_Leviathan_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Dead::Free()
{
	__super::Free();
}
