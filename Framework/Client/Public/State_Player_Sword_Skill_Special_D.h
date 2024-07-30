#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Sword_Skill_Special_D : public CState_Player
{
public:
	CState_Player_Sword_Skill_Special_D(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Sword_Skill_Special_D(const CState_Player_Sword_Skill_Special_D& rhs);
	virtual ~CState_Player_Sword_Skill_Special_D() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float4x4 m_ColMatrix;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Sword_Skill_Special_D* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END