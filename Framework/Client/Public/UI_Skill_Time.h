#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Skill_Time final : public CUI
{
public:
	CUI_Skill_Time();
	CUI_Skill_Time(const CUI_Skill_Time& rhs);
	virtual ~CUI_Skill_Time() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Skill_Time* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END