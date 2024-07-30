#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Main_Icon final : public CUI
{
public:
	CUI_Main_Icon();
	CUI_Main_Icon(const CUI_Main_Icon& rhs);
	virtual ~CUI_Main_Icon() = default;

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

public:
	static CUI_Main_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END