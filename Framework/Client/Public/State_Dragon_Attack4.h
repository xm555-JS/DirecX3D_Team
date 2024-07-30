#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_Attack4 : public CState_Boss
{
public:
	CState_Dragon_Attack4(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Attack4(const CState_Dragon_Attack4& rhs);
	virtual ~CState_Dragon_Attack4() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_bool	m_bAttack[10] = { false, };
	_float4 m_vThunderPos;


public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_Attack4* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

