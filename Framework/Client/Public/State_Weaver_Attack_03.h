#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_03 : public CState_Boss
{
public:
	CState_Weaver_Attack_03(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_03(const CState_Weaver_Attack_03& rhs);
	virtual ~CState_Weaver_Attack_03() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	void Excute_Particles(_uint _iEffectNum0, char* _szBoneName);
	void Excute_IceMeshEffect(_float fTimeDelta, _uint _KeyFrameMin, _uint _KeyFrameMax, char* _szBoneName, _bool& _bEffect);

private:
	CCollider* m_pColCom[6] = { nullptr, };
	_bool		m_bHit[6] = { false, };

	_float	m_fTimeAcc_Effect0 = 0.f;
	_float	m_fTimeAcc_Effect1 = 0.f;
	_uint	m_iEffectNum0 = 0;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);

public:
	static CState_Weaver_Attack_03* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

