#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_GaugeBar final : public CUI_Cook_WIndow
{
public:
	CUI_Cook_GaugeBar();
	CUI_Cook_GaugeBar(const CUI_Cook_GaugeBar& rhs);
	virtual ~CUI_Cook_GaugeBar() = default;

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
	_float m_fProgressRatio = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_GaugeBar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END