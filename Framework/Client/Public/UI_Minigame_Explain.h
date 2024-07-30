#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minigame_Explain final : public CUI
{
public:
	CUI_Minigame_Explain();
	CUI_Minigame_Explain(const CUI_Minigame_Explain& rhs);
	virtual ~CUI_Minigame_Explain() = default;

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
	_float m_fAccTime = 0.f;
	_float m_fAlphaRatio = 1.f;

	_float m_fSizeTime = 0.f;
	_float m_fTimeDir = 1.f;

	DECL_ONCE(m_bCreate_Explain);

private:
	void Create_Minigame_Explain();
	void MinigameWindow_Anim(_float fTimeDelta);
	void MinigameExplain_Anim(_float fTimeDelta);
	void Interact_Button(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minigame_Explain* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END