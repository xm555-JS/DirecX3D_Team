#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

public:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	void	LateTick();

public:
	HRESULT Add_ColliderGroup(ID_UINT iLayer, CCollider* pCollider);
	void	Check_Collider(ID_UINT iTempLayer, ID_UINT iSourLayer);
	void	Clear_ColliderList() { m_Layers.clear(); }
	list<CCollider*>* Find_ColliderLayer(ID_UINT iLayerID);


private:
	typedef list<CCollider*>		COLLIDERS;
	map<_uint, COLLIDERS>			m_Layers;


private:
	//void	Check_Collision(CCollider* TempCol, CCollider* SourCol);
	//_bool	Check_Sphere(CCollider* TempCol, CCollider* SourCol);
	//_bool	Check_Ray(CCollider* TempCol, CCollider* SourCol);


public:
	virtual void Free() override;
};

END