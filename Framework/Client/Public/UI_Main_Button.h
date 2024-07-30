#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Main_Button final : public CUI
{
public:
	enum MAIN_BUTTON
	{
		MAINB_ESC = 4,
		MAINB_FOOD = 15,

		MAINB_END
	};

public:
	CUI_Main_Button();
	CUI_Main_Button(const CUI_Main_Button& rhs);
	virtual ~CUI_Main_Button() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_uint m_iMainButton = MAINB_END;
	_bool m_bActive = false;

	list<class CUI_Menu_Back*>	 m_MenuBacklist;
	class CUI_Food_Window*		 m_pFoodWindow = nullptr;
	vector<class CUI_Food_Icon*> m_vecFoodIcon;
	DECL_ONCE(m_bOnce);

private:
	void Interact_Menu_Button(_float fTimeDelta);
	void Create_Food_Window(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Main_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END