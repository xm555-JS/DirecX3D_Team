#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Weapon_Skill final : public CUI
{
public:
	CUI_Weapon_Skill();
	CUI_Weapon_Skill(const CUI_Weapon_Skill& rhs);
	virtual ~CUI_Weapon_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CUI_Skill_M* m_pSkill_M = nullptr;

	DECL_ONCE(m_bOnce);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Weapon_Skill* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END