#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Quest abstract : public CUI
{
public:
	CUI_Quest();
	CUI_Quest(const CUI_Quest& rhs);
	virtual ~CUI_Quest() = default;

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
	virtual CGameObject* Clone(void* pArg) override { return nullptr; }
	virtual void Free() override;
};

END