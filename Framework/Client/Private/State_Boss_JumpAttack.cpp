#include "stdafx.h"
#include "..\Public\State_Boss_JumpAttack.h"
#include "GameInstance.h"

CState_Boss_JumpAttack::CState_Boss_JumpAttack(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Boss_JumpAttack::CState_Boss_JumpAttack(const CState_Boss_JumpAttack& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss_JumpAttack::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_IDLE);

	return S_OK;
}

HRESULT CState_Boss_JumpAttack::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Boss_JumpAttack::Enter(void* pArg)
{
	m_bEndCurState = false;
}

void CState_Boss_JumpAttack::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Boss_JumpAttack::Exit(void* pArg)
{
}

_bool CState_Boss_JumpAttack::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	// �̰� �÷��̾�� ���� ���� �ȿ� ������� ������ true�� ��ȯ�ϰ� �� ����
	if (KEY_DOWN(VK_NUMPAD1))
		return true;

	return false;
}


CState_Boss_JumpAttack* CState_Boss_JumpAttack::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Boss_JumpAttack* pInstance = new CState_Boss_JumpAttack(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Boss_JumpAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Boss_JumpAttack::Clone(void* pArg)
{
	CState_Boss_JumpAttack* pInstance = new CState_Boss_JumpAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Boss_JumpAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Boss_JumpAttack::Free()
{
}
