#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CInventory final : public CGameObject
{
public:
	typedef struct InvenDesc 
	{
		InvenDesc() {}
		InvenDesc(ITEM_TYPE _eItemType, _uint _iItemNum = 0)
		{
			eItemType = _eItemType;
			iItemNum = _iItemNum;
		}
		ITEM_TYPE	eItemType;
		_uint		iItemNum;

	}INVENDESC;

public:
	CInventory();
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	void	Add_ItemNum(_uint iAddNum) { m_tInvenDesc.iItemNum += iAddNum; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	INVENDESC	m_tInvenDesc;

public:
	static CInventory* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END