#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Attack_Target final : public CUI
{
public:
	CUI_Attack_Target();
	CUI_Attack_Target(const CUI_Attack_Target& rhs);
	virtual ~CUI_Attack_Target() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAlphaRatio = 1.f;
	_bool  m_bGlow = true;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Attack_Target* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END