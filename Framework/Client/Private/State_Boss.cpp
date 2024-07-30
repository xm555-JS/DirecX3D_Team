#include "stdafx.h"
#include "..\Public\State_Boss.h"
#include "GameInstance.h"
//#include "GameObject.h"
#include "State_Manager.h"
#include "Navigation.h"
#include "HIerarchyNode.h"

#include "Model.h"

#include "Enemy_Boss.h"

CState_Boss::CState_Boss(CState_Manager* pManager, STATE_TYPE eState)
	: CState_AI(pManager, eState)
{
}

CState_Boss::CState_Boss(const CState_Boss& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Boss::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Boss::Enter(void* pArg)
{
	m_pOwner = m_pTargetModel->Get_Owner();
	m_pOwnerTranformCom = m_pOwner->Get_Transform();
	m_pTarget = ((CEnemy_Boss*)m_pOwner)->Get_Target();

	if(nullptr != m_pOwner->Get_Component(ID_UINT(NAVIGATION)))
		m_pNaviCom = static_cast<CNavigation*>(m_pOwner->Get_Component(ID_UINT(NAVIGATION)));
}

void CState_Boss::Tick(_float fTimeDelta)
{
#if _USEMAPTOOL
	return;
#endif

	STATE_TYPE	eNextState = CState_Boss::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Boss::Tick_AnimationFix(_float fTimeDelta, string strBone, CTransform* pTransform)
{
#if _USEMAPTOOL
	return;
#endif

	STATE_TYPE	eNextState = CState_Boss::Ready_NextState();
	if (STATE_END != eNextState)
	{
		_matrix		RootBoneMat = XMLoadFloat4x4(&m_RootBoneMatrix);
		pTransform->Set_RotationMatrix(RootBoneMat);
		m_pState_Handler->Change_State(eNextState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, strBone.c_str(), &m_RootBoneMatrix);

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode(strBone.c_str());
	XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));
}

void CState_Boss::Tick_AnimationLerp(_float fTimeDelta, string strBone, CTransform* pTransform, _matrix RotateOffsetMatrix, _uint iEasingtype)
{
#if _USEMAPTOOL
	return;
#endif

	m_fTimeDelta = fTimeDelta;
	//Set_PlayerLook(fTimeDelta);

	STATE_TYPE	eNextState = CState::Ready_NextState();


	if (STATE_END != eNextState)
	{
		pTransform->Reset_RootBoneDiff();

		m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Reset_KeyFrame();

		m_pState_Handler->Change_State(eNextState);
		//m_pState_Handler->End_State(eNextState);
		//pTransform->Turn(vRotateAxis, fRotateRadian);

		m_pTargetModel->Set_RotateOffset(RotateOffsetMatrix);
		//m_pTargetModel->Set_RootEasing(true);
		m_pTargetModel->Play_Animation_Easing(fTimeDelta * m_fAnimSpeed, strBone.c_str(), &m_RootBoneMatrix, iEasingtype, pTransform);

		//m_pState_Handler->Begin_State(eNextState);

		//m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed/*, strBone.c_str(), &m_RootBoneMatrix*/);
		//m_pTargetModel->Set_ModelEasingRatio(0.2f);



		//m_pTargetModel->Set_AnimMotion(strBone.c_str());

		//m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, strBone.c_str(), &m_RootBoneMatrix);
	}
	else
	{
		m_pTargetModel->Play_Animation_Easing(fTimeDelta * m_fAnimSpeed, strBone.c_str(), &m_RootBoneMatrix, m_pTargetModel->Get_Easingtype(), pTransform);
		//m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed/*, strBone.c_str(), &m_RootBoneMatrix*/);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode(strBone.c_str());
	XMStoreFloat4x4(&m_RootBoneMatrix,/* pHierarchyNode->Get_OffsetMatrix() **/ pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix())/*pHierarchyNode->Get_TransformMatrix()*/);

	pTransform->Set_RootBoneMatrix(m_RootBoneMatrix);
}


void CState_Boss::Exit(void* pArg)
{
}

STATE_TYPE CState_Boss::Ready_NextState()
{
	for (auto& eState : m_ForceStates)
	{
		if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
			return eState;
	}

	STATE_TYPE			eState = STATE_END;
	_uint				iNum = 0;

	if (m_bEndCurState)
	{
		for (auto& eReserveState : m_ReserveStates)
		{
			if (m_pState_Manager->Get_State(eReserveState)->Check_Condition(m_pState_Handler))
			{
				eState = eReserveState;
				m_ReserveStates.erase(m_ReserveStates.begin() + iNum);
				m_ReserveStates.push_back(eState);
				break;
			}
			++iNum;
		}
	}

	return eState;
}

_bool CState_Boss::Check_Condition(CState_Handler* pHandler)
{
	return true;
}
void CState_Boss::Free()
{
}
