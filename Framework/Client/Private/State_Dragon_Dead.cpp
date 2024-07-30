#include "stdafx.h"
#include "..\Public\State_Dragon_Dead.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"

CState_Dragon_Dead::CState_Dragon_Dead(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Dead::CState_Dragon_Dead(const CState_Dragon_Dead& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Dead::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Dragon_Dead::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Dead::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 0.5f;
}

void CState_Dragon_Dead::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;

	if (2.f > m_fAccTime && 5.f > m_fAccTime)
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Roar_Fall.wav"),
			CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 1.f));

		((CEnemy*)m_pOwner)->Set_IsDissolve(true);
	}
	else if (m_fAccTime > 5.f)
	{
		GAMEINSTANCE->Add_DeadObject(m_pOwner);
		return;
	}
	else
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);

}

void CState_Dragon_Dead::Exit(void* pArg)
{
}

_bool CState_Dragon_Dead::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	if (0 >= ((CUnit*)pHandler->Get_Owner())->Get_UnitCurInfo()->fHP)
		return true;

	return false;
}

CState_Dragon_Dead* CState_Dragon_Dead::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Dead* pInstance = new CState_Dragon_Dead(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Dead::Clone(void* pArg)
{
	CState_Dragon_Dead* pInstance = new CState_Dragon_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Dead::Free()
{
	__super::Free();
}
