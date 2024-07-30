#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_2Gun_Skill_Special : public CState_Player
{
public:
	CState_Player_2Gun_Skill_Special(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_2Gun_Skill_Special(const CState_Player_2Gun_Skill_Special& rhs);
	virtual ~CState_Player_2Gun_Skill_Special() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_uint m_iCurKeyFrame = 0;
	_float m_fTimeAcc_Effect0 = 0.f;
	_float m_fTimeAcc_Effect1 = 0.f;
	_bool	m_bCreateBullet = false;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_2Gun_Skill_Special* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END