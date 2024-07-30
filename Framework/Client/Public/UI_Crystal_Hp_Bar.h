#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Crystal_Hp_Bar final : public CUI
{
public:
	CUI_Crystal_Hp_Bar();
	CUI_Crystal_Hp_Bar(const CUI_Crystal_Hp_Bar& rhs);
	virtual ~CUI_Crystal_Hp_Bar() = default;

	enum HP_TYPE
	{
		HPTYPE_BACK,
		HPTYPE_LERP,
		HPTYPE_FRONT,
		HPTYPE_NAME,

		HPTYPE_END
	};

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float		 m_fAccTime = 0.f;
	_float		 m_fSizeRatioX = 1.f;
	_float		 m_fPreHp = 0.f;

private:
	void SetUp_HpGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Crystal_Hp_Bar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END