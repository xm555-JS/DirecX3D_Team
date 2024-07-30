#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Skill_L final : public CUI
{
public:
	CUI_Skill_L();
	CUI_Skill_L(const CUI_Skill_L& rhs);
	virtual ~CUI_Skill_L() = default;

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
	 CUI*	m_pSkillTime = nullptr;
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
	static CUI_Skill_L* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END