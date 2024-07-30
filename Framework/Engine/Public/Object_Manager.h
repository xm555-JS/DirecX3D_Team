#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class CLayer;


class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	CComponent*					Get_Component(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tComponentID, _uint iIndex = 0);
	CGameObject*				Get_Object(_uint iLevelIndex, ID_UINT iLayerID, _uint iIndex = 0);
	//CLayer*						Get_Layer(_uint iLevelIndex, ID_UINT iLayerID);
	list<class CGameObject*>*	Get_Objects(_uint iLevelIndex, ID_UINT iLayerID);
	HRESULT						Get_Prototypes(ID_UINT iPrototypeID, map<_uint, CGameObject*>* pPrototypes, _uint iUseIDNum = 1);

	ID_UINT	Get_TransformID() { return m_tTransformID; }
	void	Set_TransformID(ID_UINT iID) { m_tTransformID = iID; }


public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Reserve_TickContainer(_uint iNumTickGroups);
	HRESULT Reserve_LateTickContainer(_uint iNumLateTickGroups);


public:
	HRESULT			Add_GameObject_Prototype(ID_UINT tPrototypeID, CGameObject* pPrototype);
	CGameObject*	Add_GameObject(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tPrototypeID, void* pArg = nullptr);
	CGameObject*	Add_GameObject(_uint iLevelIndex, _uint iLayerID, ID_UINT tPrototypeID, void* pArg = nullptr);
	CGameObject*	Add_GameObject(_uint iLevelIndex, _uint iLayerID, _uint iPrototypeID, void* pArg = nullptr);

	void			Add_TickGroup(_uint iGroup, CGameObject* pGameObject);
	void			Add_LateTickGroup(_uint iGroup, CGameObject* pGameObject);
	void			Remove_TickGroup(_uint iGroup, CGameObject* pGameObject);		// { m_pTickObject[iGroup]->Remove_Object(pGameObject); }
	void			Remove_LateTickGroup(_uint iGroup, CGameObject* pGameObject);	// { m_pLateTickObject[iGroup]->Remove_Object(pGameObject); }
	void			Disable_TickGroup(_uint iGroup)									{ m_pTickObjectState[iGroup] = false; }
	void			Disable_LateTickGroup(_uint iGroup)								{ m_pLateTickObjectState[iGroup] = false; }
	void			Able_TickGroup(_uint iGroup)									{ m_pTickObjectState[iGroup] = true; }
	void			Able_LateTickGroup(_uint iGroup)								{ m_pLateTickObjectState[iGroup] = true; }

	void			Add_DeadObject(CGameObject* pObject);
	void			Add_PoolingObject(CGameObject* pObject);


public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	virtual void	On_EventMessege(EVENTDESC tEvent);


public:
	void Clear(_uint iLevelIndex);


private:
	map<_uint, CGameObject*>			m_Prototypes;
	typedef map<_uint, CGameObject*>	PROTOTYPES;

private: 
	//typedef list<CGameObject*>			LAYER;
	//typedef CLayer						LAYER;
	//map<_uint, LAYER>*					m_pLayers = nullptr;
	//map<_uint, LAYER>*					m_pDisableLayers = nullptr;
	map<_uint, CLayer*>*					m_pLayers = nullptr;
	map<_uint, CLayer*>*					m_pDisableLayers = nullptr;
	typedef map<_uint, CLayer*>				LAYERS;
	//typedef map<_uint, LAYER>				LAYERS;


	_uint					m_iNumLevels = 0;

	ID_UINT					m_tTransformID;

private:
	vector<CLayer*>					m_pTickObject;
	_bool*							m_pTickObjectState;
	_uint							m_iNumTickGroups = 0;

	vector<CLayer*>					m_pLateTickObject;
	_bool*							m_pLateTickObjectState;
	_uint							m_iNumLateTickGroups = 0;

	list<CGameObject*>				m_PoolingObject;
	list<CGameObject*>				m_DeadObject;


private:
	class CGameObject* Find_Prototype(ID_UINT tPrototypeID);
	CLayer* Find_Layer(_uint iLevelIndex, ID_UINT tLayerID);
	CLayer* Find_DisableLayer(_uint iLevelIndex, ID_UINT tLayerID);
	//list<CGameObject*>* Find_Layer(_uint iLevelIndex, ID_UINT tLayerID);
	//list<CGameObject*>* Find_DisableLayer(_uint iLevelIndex, ID_UINT tLayerID);
	
private:
	//CGameObject*		Pooling_Object(CGameObject* pObject, list<CGameObject*>* pLayer, _uint iLevelIndex, ID_UINT tLayerID, ID_UINT tPrototypeID, void* pArg);
	CGameObject*		Cloning_Object(_uint iLevelIndex, ID_UINT tLayerID, ID_UINT tPrototypeID, void* pArg);
	void				Pooling_Object(CGameObject* pObject);
	void				Dead_Object(CGameObject* pObject);

public:
	virtual void Free() override;
};

END