#include "stdafx.h"
#include "..\Public\State_Dragon_Fall_Land.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Fall_Land::CState_Dragon_Fall_Land(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Fall_Land::CState_Dragon_Fall_Land(const CState_Dragon_Fall_Land& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Fall_Land::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_WEAK_LOOP);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_WEAK_LOOP] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Fall_Land::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Fall_Land::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 0.5f;
}

void CState_Dragon_Fall_Land::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Roar.wav"), CSound_Manager::CHANNEL_ENEMY, 1.f));
	IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySound(TEXT("Dragon_Fall.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.1f));

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Fall_Land::Exit(void* pArg)
{
	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(1.f);

	m_pTargetModel->Set_RootEasing(false);
}

_bool CState_Dragon_Fall_Land::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Fall_Land* CState_Dragon_Fall_Land::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Fall_Land* pInstance = new CState_Dragon_Fall_Land(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Fall_Land");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Fall_Land::Clone(void* pArg)
{
	CState_Dragon_Fall_Land* pInstance = new CState_Dragon_Fall_Land(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Fall_Land");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Fall_Land::Free()
{
	__super::Free();
}
