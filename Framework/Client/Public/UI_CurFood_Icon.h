#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_CurFood_Icon final : public CUI
{
public:
	CUI_CurFood_Icon();
	CUI_CurFood_Icon(const CUI_CurFood_Icon& rhs);
	virtual ~CUI_CurFood_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_uint  m_iFoodNum = 0;

private:
	class CUI_CurFood_Num* m_pUnitsNum = nullptr;
	class CUI_CurFood_Num* m_pTensNum = nullptr;

private:
	void SetUp_CurIcon();
	void SetUp_CurFoodNum();
	void Create_CurFoodNum();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_CurFood_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END