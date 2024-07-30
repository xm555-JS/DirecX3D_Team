#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_Button final : public CUI_Cook_WIndow
{
public:
	enum COOK_BUTTON
	{
		COOKB_EXIT,
		COOKB_COOKING = 5,
		COOKB_BACKNUM = 9, 
		COOKB_FRONTNUM = 10,
		COOKB_END
	};

public:
	CUI_Cook_Button();
	CUI_Cook_Button(const CUI_Cook_Button& rhs);
	virtual ~CUI_Cook_Button() = default;

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
	_uint	  m_iFoodType = ITEM_END;
	_uint	  m_iCookFoodNum = 0;

private:
	void Create_CookingButton(_float fTimeDelta);
	void Pooling_Cook_Objects();
	void Interact_Button(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END