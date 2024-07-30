#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_Dash_Bar final : public CUI
{
public:
	CUI_Dash_Bar();
	CUI_Dash_Bar(const CUI_Dash_Bar& rhs);
	virtual ~CUI_Dash_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CPlayer*	m_pPlayer = nullptr;
	_float			m_fSizeRatioX = 1.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Dash_Bar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END