#include "stdafx.h"
#include "State_Handler.h"
#include "State.h"
#include "State_Manager.h"
#include "Model.h"
#include "GameInstance.h"

CState_Handler::CState_Handler()
{
}

CState_Handler::CState_Handler(const CState_Handler& rhs)
{
	m_pState_Manager = CState_Manager::Get_Instance();
	m_pPlayer = PLAYERPTR;
	ZeroMemory(&m_arrStates, sizeof(CState*) * STATE_END);
}

_float CState_Handler::Get_KeyPressingTime(_uint iKey)
{
	auto iter = m_KeyPressings.find(iKey);

	if (iter == m_KeyPressings.end())
		return 0.f;

	return iter->second;
}

void CState_Handler::Set_Animation(STATE_TYPE eState, _uint iAnimIdx)
{
	m_AnimationIdx[eState] = iAnimIdx;

	if (m_arrStates[eState])
		__debugbreak();

	m_arrStates[eState] = m_pState_Manager->Clone_State(eState);

	if (m_arrStates[eState])
	{
		m_arrStates[eState]->Set_Model(m_pTargetModel);
		m_arrStates[eState]->Set_StateHandler(this);
	}
}

void CState_Handler::SetUp_StartState(STATE_TYPE eState, void* pArg)
{
	m_fTimeAcc = 0.f;

	m_eCurStateType = eState;
	m_ePreStateType = eState;

	m_pCurState = m_arrStates[eState];
	m_pCurState->Enter(pArg);

	m_pTargetModel->Set_CurrentAnimation(m_AnimationIdx[eState]);
}

void CState_Handler::Set_KeyPressingTime(_uint iKey, _float fTime)
{
	auto iter = m_KeyPressings.find(iKey);
	
	if (iter == m_KeyPressings.end())
	{
		m_KeyPressings.emplace(iKey, fTime);
	}
	else
	{
		iter->second = fTime;
	}
}

void CState_Handler::Add_KeyPressingTime(_uint iKey, _float fTimeDelta)
{
	auto iter = m_KeyPressings.find(iKey);

	if (iter == m_KeyPressings.end())
	{
		m_KeyPressings.emplace(iKey, fTimeDelta);
	}
	else
	{
		iter->second += fTimeDelta;
	}
}

void CState_Handler::Change_State(STATE_TYPE eState, void* pEnterArg, void* pExitArg)
{
	m_fTimeAcc = 0.f;
	m_pCurState->Exit(pExitArg);
	m_pCurState->Reset_SoundOption();

	m_ePreStateType = m_pCurState->Get_State();

	m_pCurState = m_arrStates[eState];
	m_pCurState->Reset_EndCurState();
	m_eCurStateType = eState;
	m_bKeyInput = false;

	m_KeyInput.clear();
	m_pTargetModel->Set_CurrentAnimation(m_AnimationIdx[eState]);
	m_pCurState->Enter(pEnterArg);
}

void CState_Handler::End_State(STATE_TYPE eState, void* pEnterArg, void* pExitArg)
{
	m_fTimeAcc = 0.f;
	m_pCurState->Exit(pExitArg);

	m_ePreStateType = m_pCurState->Get_State();
}

void CState_Handler::Begin_State(STATE_TYPE eState, void* pEnterArg, void* pExitArg)
{
	m_pCurState = m_arrStates[eState];
	m_pCurState->Reset_EndCurState();
	m_eCurStateType = eState;
	m_bKeyInput = false;

	m_KeyInput.clear();
	m_pTargetModel->Set_CurrentAnimation(m_AnimationIdx[eState]);
	m_pCurState->Enter(pEnterArg);
}

HRESULT CState_Handler::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CState_Handler::Initialize_Prototype()
{
	ZeroMemory(&m_arrStates, sizeof(CState*) * STATE_END);

	return S_OK;
}

void CState_Handler::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pCurState->Tick(fTimeDelta);
}

CState_Handler* CState_Handler::Create()
{
	CState_Handler* pInstance = new CState_Handler();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Handler");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CState_Handler::Clone(void* pArg)
{
	CState_Handler* pInstance = new CState_Handler(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Handler");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Handler::Free()
{
	__super::Free();

	//if (m_isCloned)
	//{
		for (auto& pState : m_arrStates)
			Safe_Release(pState);
	//}


	//Safe_Release(m_pCurState);
}
