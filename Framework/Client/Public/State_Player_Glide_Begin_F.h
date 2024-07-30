#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)
class CEffect_Base;
class CState_Player_Glide_Begin_F : public CState_Player
{
public:
	CState_Player_Glide_Begin_F(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Glide_Begin_F(const CState_Player_Glide_Begin_F& rhs);
	virtual ~CState_Player_Glide_Begin_F() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	list<CEffect_Base*> m_listEffect;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Glide_Begin_F* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END