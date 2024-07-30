#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Shortkey final : public CUI
{
public:
	CUI_Shortkey();
	CUI_Shortkey(const CUI_Shortkey& rhs);
	virtual ~CUI_Shortkey() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAlphaRatio = 0.85f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Shortkey* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END