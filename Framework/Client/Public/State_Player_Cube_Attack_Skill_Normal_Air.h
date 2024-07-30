#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Cube_Attack_Skill_Normal_Air : public CState_Player
{
public:
	CState_Player_Cube_Attack_Skill_Normal_Air(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Cube_Attack_Skill_Normal_Air(const CState_Player_Cube_Attack_Skill_Normal_Air& rhs);
	virtual ~CState_Player_Cube_Attack_Skill_Normal_Air() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_bool m_bShoot = false;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Cube_Attack_Skill_Normal_Air* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END