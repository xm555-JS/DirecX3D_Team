#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Damage_B_Back.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "HIerarchyNode.h"
CState_Enemy_HunterDog_Damage_B_Back::CState_Enemy_HunterDog_Damage_B_Back(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Damage_B_Back::CState_Enemy_HunterDog_Damage_B_Back(const CState_Enemy_HunterDog_Damage_B_Back& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Damage_B_Back::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_BATTLE] = 1;


	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Damage_B_Back::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Damage_B_Back::Enter(void* pArg)
{

	m_fAnimSpeed = 0.5f;

	Set_CurrentHp();

}

void CState_Enemy_HunterDog_Damage_B_Back::Tick(_float fTimeDelta)
{

	if (Is_AnimFinished(m_pState_Handler,m_pTargetModel))
		m_bEndCurState = true;

	STATE_TYPE	eNextState = CState::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pTargetModel->Get_Owner()->Get_Transform()->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Bone007", &m_RootBoneMatrix);
		m_pTargetModel->Set_ModelEasingRatio(0.2f);

		m_pTargetModel->Set_RootEasing(true);

	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "Bone007", &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("Bone007");
	XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));

	m_pTargetModel->Get_Owner()->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix);

}

void CState_Enemy_HunterDog_Damage_B_Back::Exit(void* pArg)
{
	Set_CurrentHp();

}

_bool CState_Enemy_HunterDog_Damage_B_Back::Check_Condition(CState_Handler* pHandler)
{
	return false;
}


CState_Enemy_HunterDog_Damage_B_Back* CState_Enemy_HunterDog_Damage_B_Back::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Damage_B_Back* pInstance = new CState_Enemy_HunterDog_Damage_B_Back(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Damage_B_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Damage_B_Back::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Damage_B_Back* pInstance = new CState_Enemy_HunterDog_Damage_B_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Damage_B_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Damage_B_Back::Free()
{
	__super::Free();
}
