#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

// 문제 터질 여지 있음 터질수 삭제 요망
#ifdef _DEBUG
#include "Imgui_Manager.h"  
#endif // _DEBUG

BEGIN(Client)

class CState_Player_Cube abstract : public CState_Player
{
public:
	CState_Player_Cube(CState_Manager* pState_Manager, STATE_TYPE eState);
	CState_Player_Cube(const CState_Player_Cube& rhs);
	virtual ~CState_Player_Cube() = default;


public:
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

protected:
	virtual void CreateCubeBullet();

public:
	virtual CState* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END