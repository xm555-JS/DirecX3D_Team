#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Talk_Window final : public CUI
{
public:
	CUI_Talk_Window();
	CUI_Talk_Window(const CUI_Talk_Window& rhs);
	virtual ~CUI_Talk_Window() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

private:
	void Talk_Arrow_Animation(_float fTimeDelta);

private:
	_float m_fAccTime = 0.f;
	_float m_fDir = 1.f;
	_float m_fBarSpeed = 1.f;

public:
	static CUI_Talk_Window* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END