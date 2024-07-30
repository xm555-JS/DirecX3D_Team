#include "..\Public\State_Nemesis_Patrol_Stand.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Patrol_Stand::CState_Nemesis_Patrol_Stand(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Patrol_Stand::CState_Nemesis_Patrol_Stand(const CState_Nemesis_Patrol_Stand& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Patrol_Stand::Initialize_Prototype()
{
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_PATROL_AWAKE);

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_PATROL_AWAKE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Patrol_Stand::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Patrol_Stand::Enter(void* pArg)
{
	__super::Enter(pArg);

	// �Ÿ� 10�� �ӽ� �Ÿ��� ���߿� �����÷��� ������ �÷����� Ȯ�� �غ�����
	m_fAwake_Distance = 10.f;
}

void CState_Nemesis_Patrol_Stand::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	
	// �÷��̾�� ��������ٸ� Awake_State�� ����
	if (Get_CurrentLength(m_pState_Handler) < m_fAwake_Distance)
	{
		m_bEndCurState = true; 
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Patrol_Stand::Exit(void* pArg)
{
}

_bool CState_Nemesis_Patrol_Stand::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Patrol_Stand* CState_Nemesis_Patrol_Stand::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Patrol_Stand* pInstance = new CState_Nemesis_Patrol_Stand(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Patrol_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Patrol_Stand::Clone(void* pArg)
{
	CState_Nemesis_Patrol_Stand* pInstance = new CState_Nemesis_Patrol_Stand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Patrol_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Patrol_Stand::Free()
{
	__super::Free();
}
