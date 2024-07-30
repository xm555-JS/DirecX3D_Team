#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Logo final : public CUI
{
public:
	CUI_Logo();
	CUI_Logo(const CUI_Logo& rhs);
	virtual ~CUI_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;


private:
	_float m_fAlphaRatio = 0.01f;
	_float m_fDir = 1.f;


private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Logo* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END