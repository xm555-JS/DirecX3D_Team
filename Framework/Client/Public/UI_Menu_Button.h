#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Menu_Button final : public CUI
{
public:
	enum MENU_BUTTON 
	{
		MENUB_EXIT = 2,
		MENUB_TOWERSHOP = 6,
		MENUB_SETTING = 11,
		MENUB_REPLICA = 12,
		MENUB_ORDER = 13,
		MENUB_EQUIP = 14,
		MENUB_TRAINING = 15,
		MENUB_VEHICLE = 18,
		MENUB_TERMINAL = 19,
		MENUB_INVENTORY = 20,

		MENUB_END
	};

public:
	CUI_Menu_Button();
	CUI_Menu_Button(const CUI_Menu_Button& rhs);
	virtual ~CUI_Menu_Button() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool			m_bActive = false;
	_bool			m_bRender = false;
	_float			m_fActiveTime = 0.f;
	_float			m_fAccTime = 0.f;
	_float			m_fAlphaRatio = 0.f;
	_uint			m_iMenuButton = MENUB_END;

	list<class CUI_Menu_Back*> m_MenuBacklist;
	DECL_ONCE(m_bOnce);
	DECL_ONCE(m_bptOutRect);

private:
	void SetActive_MenuButton(_float fTimeDelta);
	void Interact_MenuButton(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Menu_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END