#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Skill_M final : public CUI
{
public:
	CUI_Skill_M();
	CUI_Skill_M(const CUI_Skill_M& rhs);
	virtual ~CUI_Skill_M() = default;

public:
	_uint Get_SkillTime() { return m_iSkillTime; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	CUI*	m_pShortKey = nullptr;
	CUI*	m_pSkillTime_Units = nullptr;
	CUI*	m_pSkillTime_Tens = nullptr;
	_bool	m_bChange = false;
	_float  m_fAccTime = 0.f;
	_float	m_fDir = 1.f;
	_uint   m_iSkillTime = 0.f;

private:
	void SetUp_SkillIcon();
	void Change_SkillAnim(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Skill_M* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END