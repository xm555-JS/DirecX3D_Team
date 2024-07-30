#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Logo_Start final : public CUI
{
public:
	CUI_Logo_Start();
	CUI_Logo_Start(const CUI_Logo_Start& rhs);
	virtual ~CUI_Logo_Start() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAccTime = 0.1f;
	_float m_fDir = 1.f;

private:
	void Resizing(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Logo_Start* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END