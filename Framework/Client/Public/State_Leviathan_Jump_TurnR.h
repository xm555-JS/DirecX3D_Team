#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Leviathan_Jump_TurnR : public CState_Boss
{
public:
	CState_Leviathan_Jump_TurnR(CState_Manager* pManager, STATE_TYPE eState);
	CState_Leviathan_Jump_TurnR(const CState_Leviathan_Jump_TurnR& rhs);
	virtual ~CState_Leviathan_Jump_TurnR() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float m_fEasingTime = 0.f;
	_float4	m_vStartLook;

public:
	static CState_Leviathan_Jump_TurnR* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

