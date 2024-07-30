#include "..\Public\Collision_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}


void CCollision_Manager::LateTick()
{
	for (auto& Layer : m_Layers)
	{
		for (auto& Collider : Layer.second)
		{
			Collider->LateTick();
		}
	}
	m_Layers.clear();
}

HRESULT CCollision_Manager::Add_ColliderGroup(ID_UINT iLayer, CCollider * pCollider)
{
	if (pCollider->Is_Dead())
		return S_OK;

	auto	iter = find_if(m_Layers.begin(), m_Layers.end(), CID_Finder(iLayer.iID));

	if (iter == m_Layers.end())
	{
		list<CCollider*> pColliderList;
		pColliderList.push_back(pCollider);
		m_Layers.emplace(iLayer.iID, pColliderList);
	}
	else
	{
		iter->second.push_back(pCollider);
	}

	return S_OK;
}

void CCollision_Manager::Check_Collider(ID_UINT iTempLayer, ID_UINT iSourLayer)
{
	list<CCollider*>* Temp_ColList = Find_ColliderLayer(iTempLayer);
	list<CCollider*>* Sour_ColList = Find_ColliderLayer(iSourLayer);

	if (!Temp_ColList || !Sour_ColList)
		return;

	for (auto& Temp_elem : *Temp_ColList)
	{
		for (auto& Sour_elem : *Sour_ColList)
		{
			if (Temp_elem != Sour_elem &&
				Temp_elem->Collision(Sour_elem))
			{
				Temp_elem->OnCollision(Sour_elem);
				Sour_elem->OnCollision(Temp_elem);
			}
			//Check_Collision(Temp_elem, Sour_elem);
		}
	}
}


list<CCollider*>* CCollision_Manager::Find_ColliderLayer(ID_UINT iLayerID)
{
	auto	iter = find_if(m_Layers.begin(), m_Layers.end(), CID_Finder(iLayerID.iID));

	if (iter == m_Layers.end())
		return nullptr;

	return &(iter->second);
}

//void CCollision_Manager::Check_Collision(CCollider * TempCol, CCollider * SourCol)
//{
//	CCollider::TYPE TempType = TempCol->Get_ColliderType();
//	CCollider::TYPE SourType = SourCol->Get_ColliderType();
//
//	
//
//
//
//	if (TempType->Intersects(SourType))
//	{
//		TempCol->OnCollision(SourCol);
//		SourCol->OnCollision(TempCol);
//	}


	//if (CCollider::TYPE::LAY == TempType)
	//{
	//	if (Check_Ray(TempCol, SourCol))
	//	{
	//		TempCol->OnCollision(SourCol);
	//		SourCol->OnCollision(TempCol);
	//	}
	//}
	//else if (CCollider::TYPE::LAY == SourType)
	//{
	//	if (Check_Ray(SourCol, TempCol))
	//	{
	//		TempCol->OnCollision(SourCol);
	//		SourCol->OnCollision(TempCol);
	//	}
	//}
	//else
	//{
	//	if (Check_Sphere(TempCol, SourCol))
	//	{
	//		TempCol->OnCollision(SourCol);
	//		SourCol->OnCollision(TempCol);
	//	}
	//}
//}

//_bool CCollision_Manager::Check_Sphere(CCollider * TempCol, CCollider * SourCol)
//{
//	m_isColl = TempCol->Intersects(*SourCol->m_pSphere);
//
//
//	_float fTempRadius = TempCol->Get_Radius();
//	_float fSourRadius = SourCol->Get_Radius();
//
//	_float3 vTempPos = TempCol->Get_Owner()->Get_Pos();
//	_float3 vSourPos = SourCol->Get_Owner()->Get_Pos();
//
//	_float fLenght = D3DXVec3Length(&(vTempPos - vSourPos));
//
//	return (fLenght < (fTempRadius + fSourRadius));
//}
//
//_bool CCollision_Manager::Check_Ray(CCollider * TempCol, CCollider * SourCol)
//{
//	_float	fRayRadius = TempCol->Get_Radius();
//	_float	fSourRadius = SourCol->Get_Radius();
//
//	_float3 vRay = TempCol->Get_Ray();
//
//	_float3 vTempPos = TempCol->Get_Owner()->Get_Pos();
//	_float3 vSourPos = SourCol->Get_Owner()->Get_Pos();
//
//	_float3 vSourDir = vSourPos - vTempPos;
//
//	D3DXVec3Normalize(&vRay, &vRay);
//	
//	_float3 vRayPos = vTempPos + vRay * D3DXVec3Dot(&vSourDir, &vRay);
//
//	return  D3DXVec3Length(&(vRayPos - vSourPos)) < (fRayRadius + fSourRadius);
//}


void CCollision_Manager::Free()
{
}
