#pragma once

#include "Client_Defines.h"
#include "UI_Quest.h"

BEGIN(Client)

class CUI_Quest_Mark final : public CUI_Quest
{
public:
	CUI_Quest_Mark();
	CUI_Quest_Mark(const CUI_Quest_Mark& rhs);
	virtual ~CUI_Quest_Mark() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;


private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Quest_Mark* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END