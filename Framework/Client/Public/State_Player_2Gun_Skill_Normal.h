#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_2Gun_Skill_Normal : public CState_Player
{
public:
	CState_Player_2Gun_Skill_Normal(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_2Gun_Skill_Normal(const CState_Player_2Gun_Skill_Normal& rhs);
	virtual ~CState_Player_2Gun_Skill_Normal() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_bool	m_bCreateBullet = false;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_2Gun_Skill_Normal* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END