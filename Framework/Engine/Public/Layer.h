#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent*				Get_Component(ID_UINT iComponentID, _uint iIndex);
	class CGameObject*				Get_Object(_uint iIndex);
	list<class CGameObject*>*		Get_Objects() { return &m_GameObjects; }

	//void							Remove_Object(CGameObject* pObject) { m_RemoveObjects.push_back(pObject); }
	void							Remove_Object(CGameObject* pObject) { m_GameObjects.remove(pObject); }
	CGameObject*					Pooling_Object(ID_UINT tPrototypeID, void* pArg);
	HRESULT							Clear_Object();
	HRESULT							Remove_TickGroup();
	HRESULT							Remove_LateTickGroup();
	HRESULT							Remove_Group();
	HRESULT							Delete_Object();
	HRESULT							Delete_Object(CGameObject* pObject);
	HRESULT							Change_Level();
	HRESULT							Add_GameObject(class CGameObject* pGameObject);

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

private:
	list<class CGameObject*>			m_GameObjects;
	//list<class CGameObject*>			m_RemoveObjects;
	typedef list<class CGameObject*>	GAMEOBJECTS;

	_uint								m_iCurLevel;
	ID_UINT								m_tLayerID;

public:
	static CLayer* Create(_uint iCurLevel, ID_UINT tLayerID);
	static CLayer* CLayer::Create(_uint iCurLevel);
	virtual void Free() override;
};

END