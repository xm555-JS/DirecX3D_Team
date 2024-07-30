#include "stdafx.h"
#include "..\Public\State_Weaver_Walk_B.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Walk_B::CState_Weaver_Walk_B(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Walk_B::CState_Weaver_Walk_B(const CState_Weaver_Walk_B& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Walk_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Walk_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Walk_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Walk_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_float fDistance = XMVectorGetX(XMVector3Length(m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION) - XMLoadFloat4(&m_vTargetPos)));

	if (fDistance <= 3.f)
		m_bEndCurState = true;

	m_pOwnerTranformCom->Go_Back(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed, m_pNaviCom);


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Walk_B::Exit(void* pArg)
{
}

_bool CState_Weaver_Walk_B::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (1.5f > fLength)
		return true;

	return false;
}

CState_Weaver_Walk_B* CState_Weaver_Walk_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Walk_B* pInstance = new CState_Weaver_Walk_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Walk_B::Clone(void* pArg)
{
	CState_Weaver_Walk_B* pInstance = new CState_Weaver_Walk_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Walk_B::Free()
{
	__super::Free();
}
