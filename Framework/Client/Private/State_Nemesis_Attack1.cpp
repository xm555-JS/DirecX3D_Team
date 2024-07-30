#include "..\Public\State_Nemesis_Attack1.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "HIerarchyNode.h"

CState_Nemesis_Attack1::CState_Nemesis_Attack1(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Attack1::CState_Nemesis_Attack1(const CState_Nemesis_Attack1& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Attack1::Initialize_Prototype()
{
	//// 다음으로 넘어갈 수 있는 state
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	//// 다음 state로 넘어갈 확률
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Attack1::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Attack1::Enter(void* pArg)
{
	__super::Enter(pArg);
	//m_pTargetModel->Set_RootEasing(true);
}

void CState_Nemesis_Attack1::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	STATE_TYPE	eNextState = CState::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pOwnerTranformCom->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root", &m_RootBoneMatrix);
		//m_pTargetModel->Set_ModelEasingRatio(0.2f);

		m_pTargetModel->Set_RootEasing(true);
	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Root", &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("Root");
	XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));
	m_pOwnerTranformCom->Set_RootBoneMatrix(m_RootBoneMatrix, m_pNaviCom);
}

void CState_Nemesis_Attack1::Exit(void* pArg)
{
}

_bool CState_Nemesis_Attack1::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Attack1* CState_Nemesis_Attack1::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Attack1* pInstance = new CState_Nemesis_Attack1(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Attack1::Clone(void* pArg)
{
	CState_Nemesis_Attack1* pInstance = new CState_Nemesis_Attack1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Attack1::Free()
{
	__super::Free();
}
