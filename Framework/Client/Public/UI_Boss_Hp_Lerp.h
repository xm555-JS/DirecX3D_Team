#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Boss_Hp_Lerp final : public CUI
{
public:
	CUI_Boss_Hp_Lerp();
	CUI_Boss_Hp_Lerp(const CUI_Boss_Hp_Lerp& rhs);
	virtual ~CUI_Boss_Hp_Lerp() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float	m_fPreHp = 0.f;
	_float	m_fSizeRatioX = 1.f;
	_float	m_fAccTime = 0.f;
	_bool	m_bRender = true;

private:
	void SetUp_HpGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Boss_Hp_Lerp* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END