#pragma once

#include "Client_Defines.h"
#include "State.h"



BEGIN(Client)

class CState_AI abstract : public CState
{
public:
	CState_AI(CState_Manager* pManager, STATE_TYPE eState);
	CState_AI(const CState_AI& rhs);
	virtual ~CState_AI() = default;

public:
	_float	Get_Probability() { return m_fProbability; }
	_float	Get_PlayTime() { return m_fPlayTime; }
	_bool	Get_CheckSkillOnce() { return m_bOnceSkill; }

	void	Set_Probability(_float fProbability) { m_fProbability = fProbability; }
	void	Set_CheckSkillOnce() { m_bOnceSkill = false; }

public:
	virtual void	Tick(_float fTimeDelta) override;


protected:
	_float		m_fProbability = 0.f;
	_float		m_ReserveProbabilitys[STATE_END];

	_float		m_fPlayTime = 0.f;
	_bool		m_bOnceSkill = false;

	_bool		m_bHit = false;

	_float		m_fDamage = 0.f;


protected:
	void			Normalize_AverProbability(vector<STATE_TYPE>* AbleStates, vector<_float>* AbleRate);
	STATE_TYPE		Ready_NextState();
	void			Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName = "");



public:
	virtual void Free() override;
};

END