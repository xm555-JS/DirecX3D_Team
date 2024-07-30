#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Skill_Gauge final : public CUI
{
public:
	CUI_Skill_Gauge();
	CUI_Skill_Gauge(const CUI_Skill_Gauge& rhs);
	virtual ~CUI_Skill_Gauge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fGauge = 0.05f;
	_float m_fAccTime = 0.f;
	_bool  m_bResetGauge = false;

	class CPlayer* m_pPlayer = nullptr;

private:
	void SetUp_SkillGauge();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Skill_Gauge* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END