#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Leviathan abstract : public CState_Boss
{
public:
	typedef enum Leviathan_Skill
	{
		SKILL_FIREBALL,
		SKILL_THUNDER,
		SKILL_ICESPEAR,

		SKILL_END
	}LEVIATHAN_SKILL;

public:
	CState_Leviathan(CState_Manager* pManager, STATE_TYPE eState);
	CState_Leviathan(const CState_Leviathan& rhs);
	virtual ~CState_Leviathan() = default;

public:
	void	Use_Skill(LEVIATHAN_SKILL eSkill, _float fTimeDelta);

protected:
	_float	m_fTimeAcc[SKILL_END];
	_float	m_fCreateTime[SKILL_END];

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END


