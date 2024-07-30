#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Alert_Icon final : public CUI
{
public:
	CUI_Alert_Icon();
	CUI_Alert_Icon(const CUI_Alert_Icon& rhs);
	virtual ~CUI_Alert_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool  m_bAppear = false;

	_float m_fAlphaRatio = 0.f;
	_float m_fAppearTime = 0.f;
	_float m_fDisappearTime = 0.f;

private:
	void Appear_Alert_Icon(_float fTimeDelta);
	void Disappear_Alert_Icon(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Alert_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END