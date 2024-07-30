#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Loading_Bar final : public CUI
{
public:
	CUI_Loading_Bar();
	CUI_Loading_Bar(const CUI_Loading_Bar& rhs);
	virtual ~CUI_Loading_Bar() = default;

public:
	void Set_Finish_Loading() { m_bFinish_Loading = true; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool  m_bFinish_Loading = false;
	_float m_fSizeRatioX = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Loading_Bar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END