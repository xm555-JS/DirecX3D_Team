#include "..\Public\State_Nemesis_Walk.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Walk::CState_Nemesis_Walk(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Walk::CState_Nemesis_Walk(const CState_Nemesis_Walk& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Walk::Initialize_Prototype()
{
	//// �������� �Ѿ �� �ִ� state
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	//// ���� state�� �Ѿ Ȯ��
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Walk::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Walk::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fStartLength = 10.f;
	m_fStopLength = 3.f;
}

void CState_Nemesis_Walk::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_bool bFollow = Follow_Player(m_pState_Handler, m_pTargetModel, m_fSpeed, fTimeDelta, m_fStopLength, m_fStartLength);

	// �Ÿ��� ��������� �ٸ� ���·� �Ѿ
	if (bFollow == false)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Walk::Exit(void* pArg)
{
}

_bool CState_Nemesis_Walk::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	// ���� �÷��̾�� �Ÿ��� ���� �̻��ϸ� �� ���¸� �����ϵ���
	if (Get_CurrentLength(pHandler) >= m_fStartLength)
	{
		return true;
	}

	return false;
}

CState_Nemesis_Walk* CState_Nemesis_Walk::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Walk* pInstance = new CState_Nemesis_Walk(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Walk::Clone(void* pArg)
{
	CState_Nemesis_Walk* pInstance = new CState_Nemesis_Walk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Walk::Free()
{
	__super::Free();
}
