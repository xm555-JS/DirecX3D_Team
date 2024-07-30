#pragma once

#include "Client_Defines.h"
#include "UI_Inventory.h"

BEGIN(Client)

class CUI_Inven_Button final : public CUI_Inventory
{
public:
	enum INVEN_BUTTON
	{
		INVENB_EXIT = 14,

		INVENB_END
	};

public:
	CUI_Inven_Button();
	CUI_Inven_Button(const CUI_Inven_Button& rhs);
	virtual ~CUI_Inven_Button() = default;

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
	_uint m_iInvenButton = INVENB_END;

private:
	void Interact_Button(_float fTimeDelta);
	void Pooling_Inven_Objects();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Inven_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END