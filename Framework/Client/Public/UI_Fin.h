#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Fin final : public CUI
{
public:
	CUI_Fin();
	CUI_Fin(const CUI_Fin& rhs);
	virtual ~CUI_Fin() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAlphaRatio = 0.f;
	_float m_fAccTime = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Fin* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END