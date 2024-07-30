#include "stdafx.h"
#include "..\Public\State_AI.h"
#include "GameInstance.h"
#include "State_Manager.h"
#include "Channel.h"
#include "Unit.h"


CState_AI::CState_AI(CState_Manager* pManager, STATE_TYPE eState)
	: CState(pManager, eState)
{
	ZeroMemory(m_ReserveProbabilitys, sizeof(_float) * STATE_END);
}

CState_AI::CState_AI(const CState_AI& rhs)
	: CState(rhs)

{
	memcpy(m_ReserveProbabilitys, rhs.m_ReserveProbabilitys, sizeof(_float) * STATE_END);
}

void CState_AI::Tick(_float fTimeDelta)
{
#if _USEMAPTOOL
	return;
#endif

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}


void CState_AI::Normalize_AverProbability(vector<STATE_TYPE> * AbleStates, vector<_float>* AbleRate)
{
	_float fTotal = 0.f;
	for (auto& States : *AbleStates)
		fTotal += m_ReserveProbabilitys[States];


	for (auto& States : *AbleStates)
	{
		AbleRate->push_back(m_ReserveProbabilitys[States] / fTotal);
	}
}

STATE_TYPE CState_AI::Ready_NextState()
{
	for (auto& eState : m_ForceStates)
	{
		if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
			return eState;
	}

	if (m_bEndCurState)
	{
		_float				fRandomNum = fRandom(0.f, 1.f);
		vector<STATE_TYPE>	AbleStates;
		vector<_float>		AbleRate;

		for (auto& eState : m_ReserveStates)
		{
			if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
				AbleStates.push_back(eState);
		}
		Normalize_AverProbability(&AbleStates, &AbleRate);

		_float	fCheckNum = 0.f;
		for (size_t i = 0; i < AbleRate.size(); ++i)
		{
			fCheckNum += AbleRate[i];
			if (fRandomNum < fCheckNum)
				return AbleStates[i];
		}
	}

	return STATE_END;
}

void CState_AI::Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName)
{
	if (false == *bCheckHit)
	{
		if (iStartKeyFrame <= pTargetAnimation->GetChannel()[0]->Get_NumCurrentKeyFrame() &&
			iEndKeyFrame >= pTargetAnimation->GetChannel()[0]->Get_NumCurrentKeyFrame())

		{
			for (auto& pTargetCol : *pCollider->Get_StayColliders())
			{
				_float4x4 m = *(pTargetCol->Get_Owner()->Get_WorldMatrix());
				_vector v = pTargetCol->Get_Position();
				((CUnit*)pTargetCol->Get_Owner())->Damaged(eElement, fPower);
				if (EffectName != "")
					EFFECT_POS(EffectName, pTargetCol->Get_Owner()->Get_WorldMatrix(), pTargetCol->Get_Position());

				*bCheckHit = true;
			}

			for (auto& pTargetCol : *pCollider->Get_EnterColliders())
			{
				((CUnit*)pTargetCol->Get_Owner())->Damaged(eElement, fPower);
				if (EffectName != "")
					EFFECT_POS(EffectName, pTargetCol->Get_Owner()->Get_WorldMatrix(), pTargetCol->Get_Position());

				*bCheckHit = true;
			}
		}
	}
}





void CState_AI::Free()
{
	__super::Free();
}
