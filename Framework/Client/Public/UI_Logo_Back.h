#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Logo_Back final : public CUI
{
public:
	CUI_Logo_Back();
	CUI_Logo_Back(const CUI_Logo_Back& rhs);
	virtual ~CUI_Logo_Back() = default;

public:
	void Set_FadeOut() { m_bFadeOut = true; }

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
	_bool  m_bFadeOut = false;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Logo_Back* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END