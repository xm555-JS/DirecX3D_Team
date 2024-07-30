#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Energy_Gear final : public CUI
{
public:
	CUI_Energy_Gear();
	CUI_Energy_Gear(const CUI_Energy_Gear& rhs);
	virtual ~CUI_Energy_Gear() = default;

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
	static CUI_Energy_Gear* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END