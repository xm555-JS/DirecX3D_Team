#pragma once

#include "Client_Defines.h"
#include "UI_Inventory.h"

BEGIN(Client)

class CUI_Item_Detail final : public CUI_Inventory
{
public:
	CUI_Item_Detail();
	CUI_Item_Detail(const CUI_Item_Detail& rhs);
	virtual ~CUI_Item_Detail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Item_Detail* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END