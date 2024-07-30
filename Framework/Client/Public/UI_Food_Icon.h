#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Food_Icon final : public CUI
{
public:
	CUI_Food_Icon();
	CUI_Food_Icon(const CUI_Food_Icon& rhs);
	virtual ~CUI_Food_Icon() = default;

public:
	void Set_FoodNum(_uint iNum) { m_iFoodNum = iNum; }
	void Add_FoodNum(_uint iAddNum) { m_iFoodNum += iAddNum; }

	_uint Get_FoodNum() { return m_iFoodNum; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAlphaRatio = 0.f;
	_float m_fAccTime = 0.f;
	_uint  m_iFoodNum = 0;

private:
	class CUI_Food_Num* m_pUnitsNum = nullptr;
	class CUI_Food_Num* m_pTensNum = nullptr;

private:
	void Resize_FoodIcon(_float fTimeDelta);
	void Create_FoodNum();

	void SetUp_FoodNum();
	void SetUp_CurFood_Icon();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Food_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END