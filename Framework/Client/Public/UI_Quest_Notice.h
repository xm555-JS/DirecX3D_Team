#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Quest_Notice final : public CUI
{
public:
	CUI_Quest_Notice();
	CUI_Quest_Notice(const CUI_Quest_Notice& rhs);
	virtual ~CUI_Quest_Notice() = default;

public:
	void Set_AccTime(_float fTime) { m_fAccTime = fTime; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;
	
private:
	_float m_fWaitTime = 0.f;
	_float m_fAccTime = 0.f;
	_float m_fAlphaRatio = 1.f;

private:
	void SetUp_Symbol_Anim(_float fTimeDelta);
	void SetUp_Text_Anim(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Quest_Notice* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END