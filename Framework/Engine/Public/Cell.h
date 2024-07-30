#pragma once

#include "Base.h"

// 플레이어가 들어가는 Cell 하나에 대한 정보.
// 이웃의 정보를 가지고 있다.

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
typedef struct TagCellDesc
{
	_uint	iIndex = -1;
	_float3 vPlaneNormal = { 0.f, 0.f, 0.f };
	_uint	iProperty = 0;
	_int	iNeighbor[3] = { -1, -1, -1 };
	_float3	vLine[3] = { _float3(-1.f, -1.f, -1.f), };
	_float3	vNormal[3] = { _float3(-1.f, -1.f, -1.f), };
	_float3	vPoints[3] = { _float3(-1.f, -1.f, -1.f), };

}CELLDESC;

public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	CCell();
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) { return XMLoadFloat3(&m_vPoints[ePoint]); }
	_vector Get_Line(LINE eLine) { return XMLoadFloat3(&m_vLine[eLine]); }
	_vector Get_Normal(LINE eLine) { return XMLoadFloat3(&m_vNormal[eLine]); }
	_vector Get_PlaneNormal() { return XMLoadFloat3(&m_vPlaneNormal); }
	_int	Get_Index() const { return m_iIndex; }
	_int	Get_Neighbor(LINE eLine) const { return m_iNeighbor[eLine]; }
	_uint	Get_Property() { return m_iProperty; }
	_float3 Get_Center() { return m_vCenter; }

	void Set_Neighbor(LINE eLine, _int iNeighborIndex) { m_iNeighbor[eLine] = iNeighborIndex; }
	void Set_LineNormal(LINE eLine, _vector vNormal) { XMStoreFloat3(&m_vNormal[eLine], vNormal); }
	void Set_PlaneNormal(_float3 vNormal) { m_vPlaneNormal = vNormal; }
	void Set_Property(_uint iProperty) { m_iProperty = iProperty; }

//#ifdef _DEBUG
	class CVIBuffer_Cell* Get_Buffer() { return m_pVIBuffer; }
//#endif

public:
	HRESULT Initialize(const _float3 * pPoints, _int iIndex);
	HRESULT Initialize(CELLDESC CellDesc);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint, _int iSourIdx, _int iDestIdx, _bool* bReverseState);
	_bool isIn(_fvector vPosition, _int * pNeighborIndex);
	_bool CCell::Is_GetSlideDir(_fvector vPosition_Cur, _fvector vPosition_Next, _vector * vDirRtn, _int * iNeighborIndexRtn);

//#ifdef _DEBUG
public:
	HRESULT Render();
	HRESULT Render_Able(_float4 vPos);
//#endif // _DEBUG

private:
	_uint						m_iIndex = -1;
	_float3						m_vPoints[POINT_END] = { _float3(-1.f, -1.f, -1.f), };
	_float3						m_vLine[LINE_END] = { _float3(-1.f, -1.f, -1.f), };
	_float3						m_vNormal[LINE_END] = { _float3(-1.f, -1.f, -1.f), };
	_int						m_iNeighbor[LINE_END] = { -1, -1, -1 };

	// 이웃 Cell과의 각도.
	_float3						m_vPlaneNormal = { 0.f, 0.f, 0.f };
	// 속성
	_uint						m_iProperty = 0;

	//CELLDESC					m_CellDesc;

//#ifdef _DEBUG
	class CVIBuffer_Cell* m_pVIBuffer = nullptr;
	class CCollider*	m_pCollider = nullptr;
	_float3				m_vCenter;

	// Color
	BasicEffect* m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	BoundingSphere* m_pColSphere;
	//class CShader* m_pShaderCell = nullptr;
//#endif

public:
	static CCell* Create(const _float3 * pPoints, _int iIndex);
	static CCell* Create(CELLDESC& CellDesc);
	virtual void Free() override;
};

END