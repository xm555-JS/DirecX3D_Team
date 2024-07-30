#include "stdafx.h"
#include "..\Public\State_Boss_KeepAttack.h"
#include "GameInstance.h"

CState_Boss_KeepAttack::CState_Boss_KeepAttack(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Boss_KeepAttack::CState_Boss_KeepAttack(const CState_Boss_KeepAttack& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss_KeepAttack::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_IDLE);

	return S_OK;
}

HRESULT CState_Boss_KeepAttack::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Boss_KeepAttack::Enter(void* pArg)
{
	m_bEndCurState = false;
}

void CState_Boss_KeepAttack::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Boss_KeepAttack::Exit(void* pArg)
{
}

_bool CState_Boss_KeepAttack::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	// �̰� �÷��̾�� ���� ���� �ȿ� ������� ������ true�� ��ȯ�ϰ� �� ����
	if (KEY_DOWN(VK_NUMPAD1))
		return true;

	return false;
}


CState_Boss_KeepAttack* CState_Boss_KeepAttack::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Boss_KeepAttack* pInstance = new CState_Boss_KeepAttack(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Boss_KeepAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Boss_KeepAttack::Clone(void* pArg)
{
	CState_Boss_KeepAttack* pInstance = new CState_Boss_KeepAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Boss_KeepAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Boss_KeepAttack::Free()
{
}
