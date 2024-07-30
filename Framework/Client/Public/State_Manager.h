#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CState;

class CState_Manager : public CBase
{
public:
	DECLARE_SINGLETON(CState_Manager)

private:
	CState_Manager();
	virtual ~CState_Manager() = default;

public:
	CState*		Get_State(STATE_TYPE eState){return m_States[eState];}
	CState*		Clone_State(STATE_TYPE eState, void* pArg = nullptr);

public:
	HRESULT		Initialize();

public:
	CState*		m_States[STATE_END];



public:
	virtual void Free();
};

END