#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Boss_Hp_Back final : public CUI
{
public:
	CUI_Boss_Hp_Back();
	CUI_Boss_Hp_Back(const CUI_Boss_Hp_Back& rhs);
	virtual ~CUI_Boss_Hp_Back() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool m_bRender = true;

private:
	void SetUp_Hp_Texture();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Boss_Hp_Back* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END