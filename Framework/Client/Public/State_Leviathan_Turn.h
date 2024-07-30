#pragma once

#include "Client_Defines.h"
#include "State_Leviathan.h"

BEGIN(Client)

class CState_Leviathan_Turn : public CState_Leviathan
{
public:
	CState_Leviathan_Turn(CState_Manager* pManager, STATE_TYPE eState);
	CState_Leviathan_Turn(const CState_Leviathan_Turn& rhs);
	virtual ~CState_Leviathan_Turn() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

	void	SetUp_EnterInfo();

private:
	_float m_fEasingTime = 0.f;

	_float4	m_vStartLook;
	_float4	m_vEndLook;

	_float4	m_vCurPos;

	_float	m_fStartHight = 0.f;

	_float4	m_vStartPos;
	_float4	m_vEndPos;

	_bool	m_bSetupEnterState = false;

	_uint	m_iStartKeyFrame = 0;


public:
	static CState_Leviathan_Turn* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

