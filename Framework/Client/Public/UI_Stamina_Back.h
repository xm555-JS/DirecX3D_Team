#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Stamina_Back final : public CUI
{
public:
	CUI_Stamina_Back();
	CUI_Stamina_Back(const CUI_Stamina_Back& rhs);
	virtual ~CUI_Stamina_Back() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CPlayer*  m_pPlayer = nullptr;
	_float			m_fAccTime = 0.f;
	_float			m_fShowTime = 0.f;

	_float			m_fSizeRatioY= 1.f;
	_float			m_fPreStamina = 0.f;
	_float			m_fAlphaRatio = 0.f;

	_bool			m_bDcrsStamina = false;
	_bool			m_bIncrsStamina = true;

private:
	void SetUp_ViewPortPos();
	void SetUp_StaminaGauge(_float fTimeDelta);
	void Show_StaminaGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Stamina_Back* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END