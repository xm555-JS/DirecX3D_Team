#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Guide_Contents final : public CUI
{
public:
	CUI_Guide_Contents();
	CUI_Guide_Contents(const CUI_Guide_Contents& rhs);
	virtual ~CUI_Guide_Contents() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fBright = 1.f;
	_float m_fAccTime = 0.f;
	_float m_fDir = 1.f;

	_float m_fInitTime = 0.f;
	_float m_fAlphaRatio = 0.f;

private:
	void SetUp_Bright(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Guide_Contents* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END