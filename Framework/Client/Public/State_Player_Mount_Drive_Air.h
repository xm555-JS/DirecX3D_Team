#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Mount_Drive_Air : public CState_Player
{
public:
	CState_Player_Mount_Drive_Air(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Mount_Drive_Air(const CState_Player_Mount_Drive_Air& rhs);
	virtual ~CState_Player_Mount_Drive_Air() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float m_fTimeAcc_Effect0 = 0.f;
	list<CEffect_Base*>		m_EffectList;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override; 
	virtual void	Set_PlayerLook(_float fTimeDelta);

public:
	static CState_Player_Mount_Drive_Air* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END