#pragma once

#include "Client_Defines.h"
#include "UI_Solid.h"

BEGIN(Client)

class CUI_Enemy_Hp_Bar final : public CUI_Solid
{
	enum HPTYPE {
		HPTYPE_BACK,
		HPTYPE_FRONT,
		HPTYPE_LERP,
		HPTYPE_BACK_CRYSTAL,
		HPTYPE_FRONT_CRYSTAL,
		HPTYPE_LERP_CRYSTAL,

		HPTYPE_END
	};

public:
	CUI_Enemy_Hp_Bar();
	CUI_Enemy_Hp_Bar(const CUI_Enemy_Hp_Bar& rhs);
	virtual ~CUI_Enemy_Hp_Bar() = default;

public:
	_float Get_Dist() { return m_fCurDist; }

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
	_float		 m_fCurDist = 0.f;

private:
	void SetUp_Pos();
	void SetUp_HpGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Enemy_Hp_Bar* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END