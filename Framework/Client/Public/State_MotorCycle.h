#pragma once

#include "Client_Defines.h"
#include "State.h"

// 문제 터질 여지 있음 터질수 삭제 요망
#ifdef _DEBUG
#include "Imgui_Manager.h"  
#endif // _DEBUG


BEGIN(Engine)
class CCollider;
END

BEGIN(Client)


class CState_MotorCycle abstract : public CState
{
public:
	CState_MotorCycle(CState_Manager* pState_Manager, STATE_TYPE eState);
	CState_MotorCycle(const CState_MotorCycle& rhs);
	virtual ~CState_MotorCycle() = default;

public:
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

protected:
	_float m_fTimeDelta = 0.f;

protected:
	void	Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName = "");

public:
	virtual CState* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END