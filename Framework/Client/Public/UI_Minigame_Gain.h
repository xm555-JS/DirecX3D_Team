#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minigame_Gain final : public CUI
{
public:
	CUI_Minigame_Gain();
	CUI_Minigame_Gain(const CUI_Minigame_Gain& rhs);
	virtual ~CUI_Minigame_Gain() = default;

public:
	enum TIMER_TYPE
	{
		TIME_UP,
		TIME_DOWN,

		TIME_END
	};


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
	_int  m_iMinute = 0;
	_int  m_iSecond = 0;

	_float m_fAccTime = 0.f;

	DECL_ONCE(m_bSettingTime);

private:
	void SetUp_Timer(TIMER_TYPE eType, _float fTimeDelta, _int iMinute = 0, _int iSecond = 0);


private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minigame_Gain* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END