#include "stdafx.h"
#include "..\Public\State_Weaver_Dead.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy.h"
#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Dead::CState_Weaver_Dead(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Dead::CState_Weaver_Dead(const CState_Weaver_Dead& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Dead::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Weaver_Dead::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Dead::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Dead::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

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

void CState_Weaver_Dead::Exit(void* pArg)
{
}

_bool CState_Weaver_Dead::Check_Condition(CState_Handler* pHandler)
{
	if(0 >= ((CUnit*)pHandler->Get_Owner())->Get_UnitCurInfo()->fHP)
		return true;

	return false;
}

CState_Weaver_Dead* CState_Weaver_Dead::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Dead* pInstance = new CState_Weaver_Dead(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Dead::Clone(void* pArg)
{
	CState_Weaver_Dead* pInstance = new CState_Weaver_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Dead::Free()
{
	__super::Free();
}
