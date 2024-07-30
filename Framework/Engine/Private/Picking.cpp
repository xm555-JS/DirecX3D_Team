#include "..\Public\Picking.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	USEGAMEINSTANCE;
	m_pDevice = DEVICE;

	return S_OK;
}

void CPicking::Compute_RayInWorldSpace()
{
	if (nullptr == m_pDevice)
		return;

	/*1. 뷰포트 상의 마우스 좌표를 구하자.  */
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	VIEWPORTDESC	tViewport;

	USEGAMEINSTANCE;
	tViewport = pGameInstance->Get_Viewport();

	/* 2. 투영 스페이스 상의 마우스 좌표를 구하자. */
	// -1 에서 +1 사이의 값으로 만들어준다.
	_vector		vProjPos = {0.f, 0.f, 0.f, 0.f};
	vProjPos = XMVectorSetX(vProjPos, ptMouse.x / (tViewport.Width * 0.5f) - 1.f);
	vProjPos = XMVectorSetY(vProjPos, ptMouse.y / -(tViewport.Height * 0.5f) + 1.f);
	vProjPos = XMVectorSetZ(vProjPos, 0.0f);

	/* 3.뷰스페이스상의 마우스 좌표를 구하자. */
	_vector		vViewPos;
	XMMATRIX	ProjMatrixInv = pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);
	vViewPos = XMVector3TransformCoord(vProjPos, ProjMatrixInv);

	/* 4.마우스레이와 마우스Pos를구하자.  */
	_vector		vRayDir, vRayPos;
	vRayDir = vViewPos - _vector{ 0.f, 0.f, 0.f, 0.f };
	vRayDir = XMVectorSetW(vRayDir, 0.f);
	vRayPos = _vector{ 0.f, 0.f, 0.f, 1.f };

	/* 5.월드로가자. */
	XMMATRIX	ViewMatrixInv = pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);
	XMStoreFloat3(&m_vRayDir, XMVector3TransformNormal(vRayDir, ViewMatrixInv));
	XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(vRayPos, ViewMatrixInv));
}

_bool CPicking::Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _vector * vOut, _float * pHight)
{
	/*  월드 공간에서 로컬 공간으로 변환  */
	XMMATRIX		WorldMatrix = pTransform->Get_WorldMatrix();

	WorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	/*  충돌 검사를 위해서 월드 공간에 있는 Ray를 로컬 공간으로 변환  */
	_vector		vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrix);
	vRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrix);
	vRayDir = XMVector3Normalize(vRayDir);

	_uint			iNumFaces = pVIBuffer->Get_NumPrimitive();
	const _float3*	pVerticesPos = pVIBuffer->Get_VerticesPos();

	_float		fDist;

	vector<_float> PickedDist;
	_vector		vPickedPos;

	for (_uint i = 0; i < iNumFaces; ++i)
	{		
		_uint3		iIndices = pVIBuffer->Get_Indices(i);		


		_vector	vPosX = XMLoadFloat3(&pVerticesPos[iIndices.ix]);
		_vector	vPosY = XMLoadFloat3(&pVerticesPos[iIndices.iy]);
		_vector	vPosZ = XMLoadFloat3(&pVerticesPos[iIndices.iz]);

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vPosX, (GXMVECTOR)vPosY, (HXMVECTOR)vPosZ, fDist))
		{
			PickedDist.push_back(fDist);
		}
	}

	if (0 == PickedDist.size())
		return false;

	sort(PickedDist.begin(), PickedDist.end());
	vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * PickedDist.front();

	if(pHight)
		*pHight = PickedDist.front();

	*vOut = XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix());
	return true;
}

_bool CPicking::Picking_Terrain(CVIBuffer* pVIBuffer, CTransform* pTransform, _vector* vOut, _float* pHight, _uint3* pOutIndices)
{
	XMMATRIX		WorldMatrix = pTransform->Get_WorldMatrix();
	WorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	_vector		vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrix);
	vRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrix);
	vRayDir = XMVector3Normalize(vRayDir);

	_uint			iNumFaces = pVIBuffer->Get_NumPrimitive();
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();

	_float		fDist;

	vector<_float>	PickedDist;
	vector<_uint3>	PickedIdx;
	_vector		vPickedPos;

	_vector vCamPos = XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition());

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pVIBuffer->Get_Indices(i);

		_vector	vPosX = XMLoadFloat3(&pVerticesPos[iIndices.ix]);
		_vector	vPosY = XMLoadFloat3(&pVerticesPos[iIndices.iy]);
		_vector	vPosZ = XMLoadFloat3(&pVerticesPos[iIndices.iz]);

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, 
												(FXMVECTOR)vRayDir, 
												(FXMVECTOR)vPosX, 
												(GXMVECTOR)vPosY, 
												(HXMVECTOR)vPosZ, fDist))
		{
			PickedDist.push_back(fDist);
			PickedIdx.push_back(iIndices);
		}
	}

	if (0 == PickedDist.size())
		return false;

	_float	fMinDist = PickedDist[0];
	_uint	iMinIdx = 0;
	for (size_t i = 0; i < PickedDist.size(); ++i)
	{
		if (fMinDist > PickedDist[i])
		{
			fMinDist = PickedDist[i];
			iMinIdx = i;
		}
	}
	vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * PickedDist[iMinIdx];

	if (pOutIndices)
		*pOutIndices = PickedIdx[iMinIdx];


	if (pHight)
		*pHight = PickedDist.front();

	*vOut = XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix());
	return true;
}

_bool CPicking::Picking(CVIBuffer* pVIBuffer, _matrix WorlMatrix, _vector* vOut, _float* fHight)
{
	XMMATRIX		WorldMatrix = WorlMatrix;
	WorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	_vector		vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrix);
	vRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrix);
	vRayDir = XMVector3Normalize(vRayDir);

	_uint			iNumFaces = pVIBuffer->Get_NumPrimitive();
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();

	_float		fDist;

	vector<_float> PickedDist;
	_vector		vPickedPos;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pVIBuffer->Get_Indices(i);

		_vector	vPosX = XMLoadFloat3(&pVerticesPos[iIndices.ix]);
		_vector	vPosY = XMLoadFloat3(&pVerticesPos[iIndices.iy]);
		_vector	vPosZ = XMLoadFloat3(&pVerticesPos[iIndices.iz]);

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vPosX, (GXMVECTOR)vPosY, (HXMVECTOR)vPosZ, fDist))
		{
			//vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
			//*vOut = XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix());
			//return true;
			PickedDist.push_back(fDist);
		}
	}

	if (0 == PickedDist.size())
		return false;

	sort(PickedDist.begin(), PickedDist.end());
	vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * PickedDist.front();

	if (fHight)
		*fHight = PickedDist.front();

	if(vOut)
		*vOut = XMVector3TransformCoord(vPickedPos, WorlMatrix);

	return true;
}

_bool CPicking::Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _fvector _vRayPos, _fvector _vRayDir, _vector * vOut, _float * pHight)
{
	XMMATRIX		WorldMatrix = pTransform->Get_WorldMatrix();
	WorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	_vector		vRayPos, vRayDir;
	_vector		vOriginPos = _vRayPos;

	vRayPos = XMVector3TransformCoord(_vRayPos, WorldMatrix);
	vRayDir = XMVector3TransformNormal(_vRayDir, WorldMatrix);
	vRayDir = XMVector3Normalize(vRayDir);

	_uint			iNumFaces = pVIBuffer->Get_NumPrimitive();
	const _float3*	pVerticesPos = pVIBuffer->Get_VerticesPos();

	_float		fDist;

	vector<_float> PickedDist;
	_vector		vPickedPos;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pVIBuffer->Get_Indices(i);


		_vector	vPosX = XMLoadFloat3(&pVerticesPos[iIndices.ix]);
		_vector	vPosY = XMLoadFloat3(&pVerticesPos[iIndices.iy]);
		_vector	vPosZ = XMLoadFloat3(&pVerticesPos[iIndices.iz]);

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vPosX, (GXMVECTOR)vPosY, (HXMVECTOR)vPosZ, fDist))
		{
			PickedDist.push_back(fDist);
		}
	}

	if (0 == PickedDist.size())
		return false;

	sort(PickedDist.begin(), PickedDist.end());
	vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * PickedDist.front();
	_vector	vFinalPos = XMVector3TransformCoord(vPickedPos, pTransform->Get_WorldMatrix());

	_float	fFinalLength = XMVectorGetX(XMVector3Length(vFinalPos - vOriginPos));

	if (pHight)
		*pHight = fFinalLength;

	*vOut = vFinalPos;
	return true;
}

HRESULT CPicking::Picking(_float _fHight, _vector * vOut)
{
	_vector		vRayPos, vRayDir;

	vRayPos = XMLoadFloat3(&m_vRayPos);
	vRayDir = XMVector3Normalize(XMLoadFloat3(&m_vRayDir));

	_float2	vPos0, vPos1;
	_float4 vPos_Final = {0.f, 0.f, 0.f, 1.f};

	// 두 점을 지나는 직선의 방정식 x, y
	vPos0 = { XMVectorGetX(vRayPos), XMVectorGetY(vRayPos) };
	vPos1 = { XMVectorGetX(vRayDir + vRayPos), XMVectorGetY(vRayDir + vRayPos) };
	vPos_Final.x = (_fHight - (vPos0.y - (vPos1.y - vPos0.y) / 
								(vPos1.x - vPos0.x) * vPos0.x)) /
								((vPos1.y - vPos0.y) / (vPos1.x - vPos0.x));

	// 두 점을 지나는 직선의 방정식 z, y
	vPos0 = { XMVectorGetZ(vRayPos), XMVectorGetY(vRayPos) };
	vPos1 = { XMVectorGetZ(vRayDir + vRayPos), XMVectorGetY(vRayDir + vRayPos) };
	vPos_Final.z = (_fHight - (vPos0.y - (vPos1.y - vPos0.y) / 
								(vPos1.x - vPos0.x) * vPos0.x)) / 
								((vPos1.y - vPos0.y) / (vPos1.x - vPos0.x));

	*vOut = XMLoadFloat4(&vPos_Final);

	return S_OK;
}

HRESULT CPicking::Picking(_float _fHight, _fvector _vRayPos, _fvector _vRayDir, _vector* vOut)
{
	_vector		vRayPos, vRayDir;

	vRayPos = _vRayPos;
	vRayDir = XMVector3Normalize(_vRayDir);

	_float2	vPos0, vPos1;
	_float4 vPos_Final = { 0.f, 0.f, 0.f, 1.f };

	//x, y
	vPos0 = { XMVectorGetX(vRayPos), XMVectorGetY(vRayPos) };
	vPos1 = { XMVectorGetX(vRayDir + vRayPos), XMVectorGetY(vRayDir + vRayPos) };
	vPos_Final.x = (_fHight - (vPos0.y - (vPos1.y - vPos0.y) / (vPos1.x - vPos0.x) * vPos0.x)) / ((vPos1.y - vPos0.y) / (vPos1.x - vPos0.x));

	//z, y
	vPos0 = { XMVectorGetZ(vRayPos), XMVectorGetY(vRayPos) };
	vPos1 = { XMVectorGetZ(vRayDir + vRayPos), XMVectorGetY(vRayDir + vRayPos) };
	vPos_Final.z = (_fHight - (vPos0.y - (vPos1.y - vPos0.y) / (vPos1.x - vPos0.x) * vPos0.x)) / ((vPos1.y - vPos0.y) / (vPos1.x - vPos0.x));

	*vOut = XMLoadFloat4(&vPos_Final);

	return S_OK;
}

void CPicking::Free()
{
}
