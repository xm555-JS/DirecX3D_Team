#include "Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"
#include "GameObject.h"
#include "GameInstance.h"

CNavigation::CNavigation()
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG

{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationData)
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	_ulong		dwByte = 0;

	//string str(pNavigationData);
	//wstring test = towstring(JSONPATH_NAVIGATION + str + ".bin");
	HANDLE		hFile = CreateFile(pNavigationData, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	while (true)
	{
		CCell::CELLDESC		CellDesc;

		// Client에서는 이렇게만 불러주면 됨.
		ReadFile(hFile, &CellDesc, sizeof(CCell::CELLDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(CellDesc);

		_bool bIsSamePointed = false;
		if (0 == m_Cells.size()) // 최초의 Cell 일 때.
			bIsSamePointed = false;

		//for (auto& pOriCell : m_Cells)
		//{
		//	if (XMVector3Equal(pOriCell->Get_Point(CCell::POINT_A), pCell->Get_Point(CCell::POINT_A)) &&
		//		XMVector3Equal(pOriCell->Get_Point(CCell::POINT_B), pCell->Get_Point(CCell::POINT_B)) &&
		//		XMVector3Equal(pOriCell->Get_Point(CCell::POINT_C), pCell->Get_Point(CCell::POINT_C)))
		//	{
		//		// 같은 점이 있을 시.
		//		bIsSamePointed = true;
		//	}
		//	else
		//		continue; // 다른 점이면 그냥 넘어감.
		//}
		if (!bIsSamePointed)
			m_Cells.push_back(pCell);
		else
			Safe_Release(pCell);
	}


	//Check_SameCell();


	//Make_Neighbor();

	CloseHandle(hFile);

	//if (FAILED(Make_Neighbor()))
	//	return E_FAIL;


#ifdef _DEBUG
	m_pShader = CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Element, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));

	return S_OK;
}

/* 객체가 움직이는데 있어 네비게이션 상에서 움직일 수 있는가? 아닌가? : return  */
/* vPosition : 객체의 현재위치?!(x), 객체가 움직이고 난 이후의 위치. */
_bool CNavigation::isMove(_fvector vPosition)
{
	/* m_NaviDesc.m_iCurrentIndex : 현재 객체가 존재하는 쎌의 인덱스. */
	_int		iNeighborIndex = -1;


	/*1. 현재 존재하는 셀 안에서만 움직였을때  */
	if (true == m_Cells[m_NaviDesc.m_iCurrentIndex]->isIn(vPosition, &iNeighborIndex))
		return true;

	/* 현재 존재하고 있는 쎌 바깥으로 나갔다. */
	else
	{
		/*2. 나간쪽 쎌에 이웃이 존재할때 */
		if (0 <= iNeighborIndex)
		{
			vector<_int>	iNeighborIdxs;
			while (true)
			{
				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
					break;
				else
				{
					for (auto& iNeighbor : iNeighborIdxs)
					{
						if (iNeighbor == iNeighborIndex)
							return false;
					}

					iNeighborIdxs.push_back(iNeighborIndex);
				}

				if (m_NaviDesc.m_iCurrentIndex == iNeighborIndex)
					return false;

				if (0 > iNeighborIndex)
					return false;

			}

			m_NaviDesc.m_iCurrentIndex = iNeighborIndex;

			return true;
		}

		/*1. 나간쪽에 이웃이 없다면.  */
		else
		{
			return false;
		}
	}
}

_bool CNavigation::isUpperThanPlane(CTransform* pTransform, _float* pHightOut)
{
	_vector			pWorldPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector			PointA = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_A);
	_vector			PointB = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_B);
	_vector			PointC = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_C);

	_vector			Plane = XMPlaneFromPoints(PointA, PointB, PointC);

	pWorldPos = XMVectorSetW(pWorldPos, 1.f);

	_float			fCheckHight = XMVectorGetX(XMVector4Dot(pWorldPos, Plane));

	if (pHightOut)
		*pHightOut = fCheckHight;

	return (0.f < fCheckHight);
}

_fvector CNavigation::Get_LineSlidePos(_fvector vPosition_Cur, _fvector vPosition_Next)
{
	// 조건 더 달아줘야함. 삼각형특정 상황일때 이웃으로 안가고 벗어남.

#if 0
	// 소유 객체의 Navi On/Off 판단. (사다리 타기용)
	if (!m_NaviDesc.m_pOwner->IsNavigationEnable())
	{
		return vPosition_Next;
	}
#endif // 0


	// 1. 이웃이 있는 상태면 그만큼 이동.
	if (isMove(vPosition_Next)) // 이웃 있으면 그만큼 이동. 
	{
		return vPosition_Next;
	}
	//2. 이웃이 없는 상태면, 라인타기.
	else
	{
		// 포지션 받아와서 이동해야할 방향 벡터를 반환.
		//_vector vDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_int iCellIndex = m_NaviDesc.m_iCurrentIndex;
		// 방향벡터 받기 실패일 경우, 다음으로 검사할 이웃 쎌 인덱스 반환. 

		_vector vDir = vPosition_Next - vPosition_Cur;

		_uint	iTargetLineIdx = CCell::LINE::LINE_END;
		for (size_t i = 0; i < CCell::POINT::POINT_END; ++i)
		{
			_vector vTargetDir = vPosition_Next - m_Cells[iCellIndex]->Get_Point((CCell::POINT)i);
			if (0.f < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vTargetDir), XMVector3Normalize(m_Cells[iCellIndex]->Get_Normal((CCell::LINE)i)))))
			{
				iTargetLineIdx = i;
				break;
			}
		}

		if (iTargetLineIdx == CCell::LINE::LINE_END)
			__debugbreak;

		//(-0.0001f) == bius;
		_vector vTargetLine = XMVector3Normalize(m_Cells[iCellIndex]->Get_Line((CCell::LINE)iTargetLineIdx));
		_float fDest = XMVectorGetX(XMVector3Dot(vTargetLine, vDir));
		_vector vSour = XMVector3Normalize(m_Cells[iCellIndex]->Get_Normal((CCell::LINE)iTargetLineIdx)) * (-0.005f);
		_vector vPosition = vPosition_Cur + vTargetLine * fDest + vSour;

		//while (true)
		//{
		//	if (true == m_Cells[iCellIndex]->Is_GetSlideDir(vPosition_Cur, vPosition_Next, &vDir, &iCellIndex))
		//		break;

		//	if (0 > iCellIndex)
		//		break;
		//}

		//// 방향을 제외한 이동해야할 양.
		//_float vScale = XMVectorGetX(XMVector3Length(vPosition_Next - vPosition_Cur));
		//// 라인을 타서 나온 포지션 값.
		//_vector vPosition = vPosition_Cur + (vDir * vScale);

		// 3. 라인을 타서 나온 포지션 값으로 이동 한 곳으로 이동가능 하면 이동한다.
		if (isMove(vPosition))
		{
			return vPosition;
		}
		// 4. 라인 타서 나온 포지션 값으로 이동한 곳에 이동 가능한 Cell이 없으면 정지 (== 외진 모서리)
		else
		{
			return vPosition_Cur;
		}
	}
}

_fvector CNavigation::Get_Plane(CTransform* pTransform)
{
	_vector			pWorldPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector			pUp = pTransform->Get_State(CTransform::STATE_UP);

	_vector			PointA = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_A);
	_vector			PointB = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_B);
	_vector			PointC = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_C);

	_vector			Plane = XMPlaneFromPoints(PointA, PointB, PointC);

	/* ax + by + cz + d = 0 */
	//by = -ax -cz - d;
	//n = ai + bj + ck
	pWorldPos = XMVectorSetY(pWorldPos, (-XMVectorGetX(Plane) * XMVectorGetX(pWorldPos) - XMVectorGetZ(Plane) * XMVectorGetZ(pWorldPos) - XMVectorGetW(Plane)) / XMVectorGetY(Plane));

	return Plane;
}

void CNavigation::SetUp_OnTerrain(CTransform* pTransform)
{
	_vector			pWorldPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector			PointA = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_A);
	_vector			PointB = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_B);
	_vector			PointC = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_C);

	_vector			Plane = XMPlaneFromPoints(PointA, PointB, PointC);

	/* ax + by + cz + d = 0 */
	pWorldPos = XMVectorSetY(pWorldPos, (-XMVectorGetX(Plane) * XMVectorGetX(pWorldPos) - XMVectorGetZ(Plane) * XMVectorGetZ(pWorldPos) - XMVectorGetW(Plane)) / XMVectorGetY(Plane));

	pTransform->Set_State(CTransform::STATE_TRANSLATION, pWorldPos);
}

#if 0
void CNavigation::SetUp_OnTerrain_Anim(CTransform* pTransform, _matrix vStartingPointWorldMatrix)
{
	_vector			pWorldPos = pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector			PointA = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_A);
	_vector			PointB = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_B);
	_vector			PointC = m_Cells[m_NaviDesc.m_iCurrentIndex]->Get_Point(CCell::POINT_C);

	_vector			Plane = XMPlaneFromPoints(PointA, PointB, PointC);

	/* ax + by + cz + d = 0 */
	pWorldPos = XMVectorSetY(pWorldPos, (-XMVectorGetX(Plane) * XMVectorGetX(pWorldPos) - XMVectorGetZ(Plane) * XMVectorGetZ(pWorldPos) - XMVectorGetW(Plane)) / XMVectorGetY(Plane));

	_matrix WorldMatrixRendering = pTransform->Get_WorldMatrixRendering();
	WorldMatrixRendering.r[3] = XMVectorSetY(WorldMatrixRendering.r[3], XMVectorGetY(pWorldPos));
	pTransform->Set_WorldMatrixRendering(WorldMatrixRendering);
}
#endif // 0

void CNavigation::Find_CurrentIndex(CTransform* pTransform)
{
	_int		iNeighborIndex = -1;

	for (auto& iter : m_Cells)
	{
		//pWorldPosXZ 가 삼각형 내에 존재하는지 판단 //https://jieun0113.tistory.com/111 준영꾸
		//_float2			pWorldPosXZ = { XMVectorGetX(pTransform->Get_State(CTransform::STATE_TRANSLATION)), XMVectorGetZ(pTransform->Get_State(CTransform::STATE_TRANSLATION)) };
		//
		//_float2 p1 = { XMVectorGetX(iter->Get_Point(CCell::POINT_A)), XMVectorGetZ(iter->Get_Point(CCell::POINT_A)) };
		//_float2 p2 = { XMVectorGetX(iter->Get_Point(CCell::POINT_B)), XMVectorGetZ(iter->Get_Point(CCell::POINT_B)) };
		//_float2 p3 = { XMVectorGetX(iter->Get_Point(CCell::POINT_C)), XMVectorGetZ(iter->Get_Point(CCell::POINT_C)) };
		//
		//_float alpha = ((p2.y - p3.y) * (pWorldPosXZ.x - p3.x) + (p3.x - p2.x) * (pWorldPosXZ.y - p3.y)) /
		//	((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		//_float beta = ((p3.y - p1.y) * (pWorldPosXZ.x - p3.x) + (p1.x - p3.x) * (pWorldPosXZ.y - p3.y)) /
		//	((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
		//_float gamma = 1.0f - alpha - beta;
		//
		//if (0 < alpha && 0 < beta && 0 < gamma)
		//{
		//	m_NaviDesc.m_iCurrentIndex = iter->Get_Index();
		//	break;
		//}

		//재민꾸
		if (iter->isIn(pTransform->Get_State(CTransform::STATE_TRANSLATION), &iNeighborIndex))
		{
			m_NaviDesc.m_iCurrentIndex = iter->Get_Index();
			break;
		}

	}

}

// 단 이웃이 반드시 있어야 함
void CNavigation::Make_LineNormal()
{
	for (auto& pCell : m_Cells)
	{
		_vector vPlaneNormal = pCell->Get_PlaneNormal();
		_vector vNeighborPlaneNormal[CCell::LINE_END];
		vNeighborPlaneNormal[CCell::LINE_AB] = m_Cells[pCell->Get_Neighbor(CCell::LINE_AB)]->Get_PlaneNormal();
		vNeighborPlaneNormal[CCell::LINE_BC] = m_Cells[pCell->Get_Neighbor(CCell::LINE_BC)]->Get_PlaneNormal();
		vNeighborPlaneNormal[CCell::LINE_CA] = m_Cells[pCell->Get_Neighbor(CCell::LINE_CA)]->Get_PlaneNormal();

		vNeighborPlaneNormal[CCell::LINE_AB] = XMVector3Normalize(vPlaneNormal + vNeighborPlaneNormal[CCell::LINE_AB]);
		vNeighborPlaneNormal[CCell::LINE_BC] = XMVector3Normalize(vPlaneNormal + vNeighborPlaneNormal[CCell::LINE_BC]);
		vNeighborPlaneNormal[CCell::LINE_CA] = XMVector3Normalize(vPlaneNormal + vNeighborPlaneNormal[CCell::LINE_CA]);

		// 3D Normal
		pCell->Set_LineNormal(CCell::LINE_AB, XMVector3Cross(pCell->Get_Line(CCell::LINE_AB), vNeighborPlaneNormal[CCell::LINE_AB]));
		pCell->Set_LineNormal(CCell::LINE_BC, XMVector3Cross(pCell->Get_Line(CCell::LINE_BC), vNeighborPlaneNormal[CCell::LINE_BC]));
		pCell->Set_LineNormal(CCell::LINE_CA, XMVector3Cross(pCell->Get_Line(CCell::LINE_CA), vNeighborPlaneNormal[CCell::LINE_CA]));
	}
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	_float4x4		WorldMatrix;


	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);


	m_pShader->Set_RawValue("g_ViewMatrix", pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	RELEASE_INSTANCE(CPipeLine);


	// 밟은 Cell은 하얀색으로 표시.
	if (0 <= m_NaviDesc.m_iCurrentIndex)
	{
		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMMatrixTranslation(0.f, 0.1f, 0.f)));
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		m_pShader->Set_RawValue("g_vColor", &Colors::White, sizeof(_float4));
		m_pShader->Begin(0);

		m_Cells[m_NaviDesc.m_iCurrentIndex]->Render();
	}
	//else
	// 높낮이 별로 초록색 ~ 빨간색 으로 표시
	//{
	//
	//	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	//
	//	m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	//	m_pShader->Set_RawValue("g_vColor", &Colors::Red, sizeof(_float4));
	//	m_pShader->Begin(0);
	//
	//	for (auto& pCell : m_Cells)
	//	{
	//		if (nullptr != pCell)
	//			pCell->Render();
	//	}
	//
	//}



	return S_OK;
}
#endif // _DEBUG

// Cell의 이웃들을 설정한다.
HRESULT CNavigation::Make_Neighbor()
{
	//for (auto& pSourCell : m_Cells)
	//{
	//	for (auto& pDestCell : m_Cells)
	//	{
	//		if (pSourCell == pDestCell)
	//			continue;

	//		if (true == pDestCell->Compare_Points(
	//			pSourCell->Get_Point(CCell::POINT_A),
	//			pSourCell->Get_Point(CCell::POINT_B)))
	//		{
	//			pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell->Get_Index());
	//			continue;
	//		}

	//		if (true == pDestCell->Compare_Points(
	//			pSourCell->Get_Point(CCell::POINT_B),
	//			pSourCell->Get_Point(CCell::POINT_C)))
	//		{
	//			pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell->Get_Index());
	//			continue;
	//		}

	//		if (true == pDestCell->Compare_Points(
	//			pSourCell->Get_Point(CCell::POINT_C),
	//			pSourCell->Get_Point(CCell::POINT_A)))
	//		{
	//			pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell->Get_Index());
	//			continue;
	//		}
	//	}
	//}


	return S_OK;
}

void CNavigation::Check_SameCell()
{
	list<CCell*>	TempCells;

	for (size_t i = 0; i < m_Cells.size(); ++i)
	{
		TempCells.push_back(m_Cells[i]);
	}

	for (size_t i = 0; i < m_Cells.size(); ++i)
	{
		for (size_t j = i + 1; j < m_Cells.size(); ++j)
		{
			_bool	bCheck[3] = { false };
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[2] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[2] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[2] = true;
			}

			if (bCheck[0] && bCheck[1] && bCheck[2])
				TempCells.remove(m_Cells[i]);
		}
	}

	m_Cells.clear();
	for (auto& pCell : TempCells)
	{
		m_Cells.push_back(pCell);
	}
}

CNavigation* CNavigation::Create(const _tchar* pNavigationData)
{
	CNavigation* pInstance = new CNavigation();

	if (FAILED(pInstance->Initialize_Prototype(pNavigationData)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG

}
