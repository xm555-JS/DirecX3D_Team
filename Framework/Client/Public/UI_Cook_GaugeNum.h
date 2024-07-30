#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_GaugeNum final : public CUI_Cook_WIndow
{
public:
	CUI_Cook_GaugeNum();
	CUI_Cook_GaugeNum(const CUI_Cook_GaugeNum& rhs);
	virtual ~CUI_Cook_GaugeNum() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_GaugeNum* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END