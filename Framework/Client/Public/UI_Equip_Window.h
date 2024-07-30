#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Equip_Window abstract : public CUI
{
public:
	CUI_Equip_Window();
	CUI_Equip_Window(const CUI_Equip_Window& rhs);
	virtual ~CUI_Equip_Window() = default;

public:
	void Set_AccTime(_float fTime = 0.f) { m_fAccTime = fTime; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

protected:
	_float m_fAlphaRatio = 1.f;
	_float m_fAlphaTime = 0.f;
	_float m_fAccTime = 0.f;

	_float m_fSizeRatioX = 0.f;
	_float m_fSizeTime = 0.f;
	_float m_fDir = 1.f;

protected:
	void Create_Equip_Window(_float fTimeDelta);

	_bool Weapon_Select_Anim(_float fTimeDelta);
	void  SetUp_Slot_Weapon(_uint iWeaponSlot);
	void  Equip_PlayerWeapon(_uint iWeaponSlot, WEAPON_TYPE eWeaponType);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; }
	virtual void Free() override;
};

END