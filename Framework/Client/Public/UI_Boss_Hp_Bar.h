#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Boss_Hp_Bar final : public CUI
{
public:
	CUI_Boss_Hp_Bar();
	CUI_Boss_Hp_Bar(const CUI_Boss_Hp_Bar& rhs);
	virtual ~CUI_Boss_Hp_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fSizeRatioX = 1.f;
	_bool  m_bRender = true;

private:
	void SetUp_HpGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Boss_Hp_Bar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END