#pragma once

#include "Client_Defines.h"
#include "UI_Inventory.h"

BEGIN(Client)

class CUI_Item final : public CUI_Inventory
{
public:
	CUI_Item();
	CUI_Item(const CUI_Item& rhs);
	virtual ~CUI_Item() = default;

public:
	void Set_ItemNum(_uint iNum) { m_iItemNum = iNum; }
	void Add_ItemNum(_uint iAddNum) { m_iItemNum += iAddNum; }
	_uint Get_ItemNum() { return m_iItemNum; }

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
	_uint m_iItemNum = 0;
	_float m_fSizeTime = 0.f;

private:
	class CUI_Item_Num* m_pUnitsNum = nullptr;
	class CUI_Item_Num* m_pTensNum = nullptr;
	list<class CUI*> m_SelectItemlist;

private:
	void Create_ItemNum();
	void Create_ItemDetail();
	void SetUp_ItemNum();

	void Select_Item(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Item* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END