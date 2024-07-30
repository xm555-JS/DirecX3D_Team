#include "VIBuffer_Rect_Instance.h"
#include "PipeLine.h"
#include "Easing_Utillity.h"

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance()
{
}

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.0f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.0f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize(void* pArg)
{
	NULLCHECK(pArg);
	memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLE_DESC));

#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_ParticleDesc.iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;


	VTXINSTANCE* pInstance = new VTXINSTANCE[m_ParticleDesc.iNumInstance];
	m_vStartScaleRUL = new _float3[m_ParticleDesc.iNumInstance];
	//m_vCurrentScaleRUL = new _float3[m_ParticleDesc.iNumInstance];
	m_vStartSpeedXYZ = new _float3[m_ParticleDesc.iNumInstance];
	//m_vCurrentSpeedXYZ = new _float3[m_ParticleDesc.iNumInstance];
	m_fEachAccelationXYZ = new _float3[m_ParticleDesc.iNumInstance];
	m_fWholeAccelation = { m_ParticleDesc.fWholeAccelation.x, m_ParticleDesc.fWholeAccelation.y, m_ParticleDesc.fWholeAccelation.z };
	m_vStartRotateRUL = new _float3[m_ParticleDesc.iNumInstance];
	//m_vCurrentRotateSpeedRUL = new _float3[m_ParticleDesc.iNumInstance];
	m_fRotateAccelation = new _float3[m_ParticleDesc.iNumInstance];

	SetUp_ParticleDesc(m_ParticleDesc, &pInstance);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);
#pragma endregion


	Setting_Index();
	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Pooling(void* pArg)
{
	NULLCHECK(m_pContext);
	NULLCHECK(m_pVBInstance);
	NULLCHECK(pArg);

	memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLE_DESC));

	m_fTimeAcc = 0;
	m_fWholeAccelation = { m_ParticleDesc.fWholeAccelation.x, m_ParticleDesc.fWholeAccelation.y, m_ParticleDesc.fWholeAccelation.z };

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	SetUp_ParticleDesc(m_ParticleDesc, (VTXINSTANCE**)&SubResource.pData);

	m_pContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Loop(void)
{
	if (nullptr == m_pContext ||
		nullptr == m_pVBInstance)
		return E_FAIL;

	m_fTimeAcc = 0;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유효한 값이 담겨있는 형태로 얻어온다. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어온다. */
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SetUp_ParticleDesc(m_ParticleDesc, (VTXINSTANCE**)&SubResource.pData);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eToplogy);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	m_pContext->DrawIndexedInstanced(6, m_ParticleDesc.iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Rect_Instance::Update(_float fTimeDelta)
{
	if (nullptr == m_pContext ||
		nullptr == m_pVBInstance)
		return;
	m_fTimeAcc += fTimeDelta;
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유효한 값이 담겨있는 형태로 얻어온다.(for문으로 돌때) */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어온다. */
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	_matrix	ViewMatrixInv;
	ViewMatrixInv = CPipeLine::Get_Instance()->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	_vector vCurrentSpeedXYZ = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector vCurrentScaleRUL = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector vCurrentRotateSpeedRUL = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	for (_uint i = 0; i < m_ParticleDesc.iNumInstance; ++i)
	{
		vCurrentSpeedXYZ = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vCurrentScaleRUL = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		// Scale 관련
		if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iScaleEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iScaleEasing_Middle)
			{
				if (m_ParticleDesc.fMiddleScaleTimePoint > m_fTimeAcc)
				{
					vCurrentScaleRUL = CEasing_Utillity::Easing(m_ParticleDesc.iScaleEasing_Middle, XMLoadFloat3(&m_vStartScaleRUL[i]), XMLoadFloat3(&m_ParticleDesc.vMiddleScaleXYZ), m_fTimeAcc, m_ParticleDesc.fMiddleScaleTimePoint);
				}
				else
				{
					vCurrentScaleRUL = CEasing_Utillity::Easing(m_ParticleDesc.iScaleEasing_End, XMLoadFloat3(&m_ParticleDesc.vMiddleScaleXYZ), XMLoadFloat3(&m_ParticleDesc.vEndScaleXYZ), _double(m_fTimeAcc - m_ParticleDesc.fMiddleScaleTimePoint), _double(m_ParticleDesc.fLifeTime.y - m_ParticleDesc.fMiddleScaleTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				vCurrentScaleRUL = CEasing_Utillity::Easing(m_ParticleDesc.iScaleEasing_End, XMLoadFloat3(&m_vStartScaleRUL[i]), XMLoadFloat3(&m_ParticleDesc.vEndScaleXYZ), m_fTimeAcc, m_ParticleDesc.fLifeTime.y);
			}
		}
		else
		{
			vCurrentScaleRUL = XMVectorSet(m_vStartScaleRUL[i].x, m_vStartScaleRUL[i].y, m_vStartScaleRUL[i].z, 1.f);
		}

		// Moving 관련
		if (MOVING_TYPE::SPEED == m_ParticleDesc.iMovingMode)
		{
			// Speed
			if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iSpeedEasing_End) // 끝 보간이 있을 때.
			{
				// 중간 보간이 있을 때.
				if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iSpeedEasing_Middle)
				{
					if (m_ParticleDesc.fMiddleSpeedTimePoint > m_fTimeAcc)
					{
						vCurrentSpeedXYZ = CEasing_Utillity::Easing(m_ParticleDesc.iSpeedEasing_Middle, XMLoadFloat3(&m_vStartSpeedXYZ[i]), XMLoadFloat3(&m_ParticleDesc.vMiddleSpeedXYZ), m_fTimeAcc, m_ParticleDesc.fMiddleSpeedTimePoint);

					}
					else
					{
						vCurrentSpeedXYZ = CEasing_Utillity::Easing(m_ParticleDesc.iSpeedEasing_End, XMLoadFloat3(&m_ParticleDesc.vMiddleSpeedXYZ), XMLoadFloat3(&m_ParticleDesc.vEndSpeedXYZ), _double(m_fTimeAcc - m_ParticleDesc.fMiddleSpeedTimePoint), _double(m_ParticleDesc.fLifeTime.y - m_ParticleDesc.fMiddleSpeedTimePoint));
					}
				}
				else // 끝 보간만 있을 때.
				{
					vCurrentSpeedXYZ = CEasing_Utillity::Easing(m_ParticleDesc.iSpeedEasing_End, XMLoadFloat3(&m_vStartSpeedXYZ[i]), XMLoadFloat3(&m_ParticleDesc.vEndSpeedXYZ), m_fTimeAcc, m_ParticleDesc.fLifeTime.y);
				}
			}
			else
			{
				vCurrentSpeedXYZ = XMVectorSet(m_vStartSpeedXYZ[i].x, m_vStartSpeedXYZ[i].y, m_vStartSpeedXYZ[i].z, 1.f);
			}

			((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += XMVectorGetX(vCurrentSpeedXYZ) * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += XMVectorGetY(vCurrentSpeedXYZ) * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += XMVectorGetZ(vCurrentSpeedXYZ) * fTimeDelta;
		}
		else if (MOVING_TYPE::ACCELATION == m_ParticleDesc.iMovingMode)
		{
			// Each Accelation
			//v = vo + at
			vCurrentSpeedXYZ = XMVectorSet(
				m_vStartSpeedXYZ[i].x + (m_fEachAccelationXYZ[i].x + m_fWholeAccelation.x) * m_fTimeAcc,
				m_vStartSpeedXYZ[i].y + (m_fEachAccelationXYZ[i].y + m_fWholeAccelation.y) * m_fTimeAcc,
				m_vStartSpeedXYZ[i].z + (m_fEachAccelationXYZ[i].z + m_fWholeAccelation.z) * m_fTimeAcc, 0.f);

			((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += XMVectorGetX(vCurrentSpeedXYZ) * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += XMVectorGetY(vCurrentSpeedXYZ) * fTimeDelta;
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += XMVectorGetZ(vCurrentSpeedXYZ) * fTimeDelta;
		}
		else if (MOVING_TYPE::POSITIONING == m_ParticleDesc.iMovingMode)
		{
			// s = v * t
			if (m_ParticleDesc.bIsGatheringOnce) // Duration 동안 한번 모이고 끝남.
			{
				// 시작부터 최종까지의 거리를 기억했다가(제각각).
				// 걸리는 시간만큼 맞춰 속도를 넣어주면 됨(제각각).
				// 일정 거리까지는, 어떤 속도

				// 거리 나누기 시간 하면 평균 속도가 나옴.
				// 거리(+방향) => 타겟 위치 - 현재 위치.
				vCurrentSpeedXYZ = XMVectorSet(
					m_ParticleDesc.vTargetPosition.x - ((VTXINSTANCE*)SubResource.pData)[i].vPosition.x,
					m_ParticleDesc.vTargetPosition.y - ((VTXINSTANCE*)SubResource.pData)[i].vPosition.y,
					m_ParticleDesc.vTargetPosition.z - ((VTXINSTANCE*)SubResource.pData)[i].vPosition.z, 0.f);

				// Speed

			}
			else // 각각 알아서 모이는 모드 (지속시간동안 목표포지션 도달한 인스턴싱들은 반복 생성.)
			{
				vCurrentSpeedXYZ = XMVector3Normalize(XMVectorSet(m_ParticleDesc.vTargetPosition.x, m_ParticleDesc.vTargetPosition.y, m_ParticleDesc.vTargetPosition.z, 0.f) - XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition));
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.x += XMVectorGetX(vCurrentSpeedXYZ) * fabs(m_vStartSpeedXYZ[i].x) * fTimeDelta;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.y += XMVectorGetY(vCurrentSpeedXYZ) * fabs(m_vStartSpeedXYZ[i].x) * fTimeDelta;
				((VTXINSTANCE*)SubResource.pData)[i].vPosition.z += XMVectorGetZ(vCurrentSpeedXYZ) * fabs(m_vStartSpeedXYZ[i].x) * fTimeDelta;

				// 이동된 후, 새롭게 계산한 방향벡터가 바뀌었을 때 == 목표포지션을 지났을 때.  --> 시작위치, 속도를 새롭게 바꿔준다.
				_vector vDirNext = XMVector3Normalize(XMVectorSet(m_ParticleDesc.vTargetPosition.x, m_ParticleDesc.vTargetPosition.y, m_ParticleDesc.vTargetPosition.z, 0.f) - XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition));
				if (!XMVector3NearEqual(vCurrentSpeedXYZ, vDirNext, XMVectorSet(0.2f, 0.2f, 0.2f, 0.f)))
				{
					switch (m_ParticleDesc.iStartPointMode)
					{
					case STARTPOINT_TYPE::POINT:
						((VTXINSTANCE*)SubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
						vCurrentSpeedXYZ = XMVector3Normalize(XMVectorSet(m_ParticleDesc.vTargetPosition.x, m_ParticleDesc.vTargetPosition.y, m_ParticleDesc.vTargetPosition.z, 0.f) - XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition));
						break;
					case STARTPOINT_TYPE::CUBE:
						((VTXINSTANCE*)SubResource.pData)[i].vPosition = _float4(fRandom(m_ParticleDesc.vRangeXYZ_Min.x, m_ParticleDesc.vRangeXYZ_Max.x), fRandom(m_ParticleDesc.vRangeXYZ_Min.y, m_ParticleDesc.vRangeXYZ_Max.y), fRandom(m_ParticleDesc.vRangeXYZ_Min.z, m_ParticleDesc.vRangeXYZ_Max.z), 1.f);
						vCurrentSpeedXYZ = XMVector3Normalize(XMVectorSet(m_ParticleDesc.vTargetPosition.x, m_ParticleDesc.vTargetPosition.y, m_ParticleDesc.vTargetPosition.z, 0.f) - XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition));
						break;
					case STARTPOINT_TYPE::SPHERE:
					{
						_vector vPos = XMVectorSetW(XMVector3Normalize(XMVectorSet(fRandom(-1.f, 1.f), fRandom(-1.f, 1.f), fRandom(-1.f, 1.f), 0.f)) * fRandom(0.f, m_ParticleDesc.fRangeRadius), 1.f);
						XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition, vPos);
						vCurrentSpeedXYZ = XMVector3Normalize(XMVectorSet(m_ParticleDesc.vTargetPosition.x, m_ParticleDesc.vTargetPosition.y, m_ParticleDesc.vTargetPosition.z, 0.f) - vPos);
					}
					break;
					default:
						break;
					}

					m_vStartSpeedXYZ[i].x = fRandom(m_ParticleDesc.vStartSpeedXYZ_Min.x, m_ParticleDesc.vStartSpeedXYZ_Max.x);
				}
			}
		}

		// Rotation 관련.
		if (BILLBOARD_TYPE::NORMAL == m_ParticleDesc.iBillBoardMode) // Normal 
		{
			// Rotation 
			vCurrentRotateSpeedRUL = XMVectorSet(
				(m_vStartRotateRUL[i].x + m_fRotateAccelation[i].x * m_fTimeAcc) * fTimeDelta,
				(m_vStartRotateRUL[i].y + m_fRotateAccelation[i].y * m_fTimeAcc) * fTimeDelta,
				(m_vStartRotateRUL[i].z + m_fRotateAccelation[i].z * m_fTimeAcc) * fTimeDelta, 0.f);

			_vector vRadianAngles = { XMVectorGetX(vCurrentRotateSpeedRUL), XMVectorGetY(vCurrentRotateSpeedRUL), XMVectorGetZ(vCurrentRotateSpeedRUL), 0.f };

			_vector	vQuaternion;
			vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
			_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

			_vector		vRight, vUp, vLook;
			vRight = XMVector3Normalize(XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vRight)) * XMVectorGetX(vCurrentScaleRUL);
			vUp = XMVector3Normalize(XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vUp)) * XMVectorGetY(vCurrentScaleRUL);
			vLook = XMVector3Normalize(XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vLook)) * XMVectorGetZ(vCurrentScaleRUL);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vRight, XMVector3TransformNormal(vRight, RotationMatrix));
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vUp, XMVector3TransformNormal(vUp, RotationMatrix));
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vLook, XMVector3TransformNormal(vLook, RotationMatrix));
		}
		else if (BILLBOARD_TYPE::BILLBOARD == m_ParticleDesc.iBillBoardMode) // BillBoard
		{
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vRight, ViewMatrixInv.r[0] * XMVectorGetX(vCurrentScaleRUL));
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vUp, ViewMatrixInv.r[1] * XMVectorGetY(vCurrentScaleRUL));
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vLook, ViewMatrixInv.r[2] * XMVectorGetZ(vCurrentScaleRUL));
		}
		else if (BILLBOARD_TYPE::DIRECTIONAL == m_ParticleDesc.iBillBoardMode) // Directional
		{
			// 초기 각도가 들어와야함.
			_vector vUp = XMVector3Normalize(XMVectorSet(XMVectorGetX(vCurrentSpeedXYZ), XMVectorGetY(vCurrentSpeedXYZ), XMVectorGetZ(vCurrentSpeedXYZ), 0.f)) * XMVectorGetY(vCurrentScaleRUL);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vUp, vUp);
			_vector vLook = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vUp)) * XMVectorGetZ(vCurrentScaleRUL);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vLook, vLook);
			_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook)) * XMVectorGetX(vCurrentScaleRUL);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vRight, vRight);
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CVIBuffer_Rect_Instance::Setting_Index(void)
{
#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
#pragma endregion
}

void CVIBuffer_Rect_Instance::SetUp_ParticleDesc(PARTICLE_DESC Desc, VTXINSTANCE** pOutInstance)
{
	for (_uint i = 0; i < Desc.iNumInstance; ++i)
	{
		m_vStartScaleRUL[i].x = fRandom(Desc.vStartScaleXYZ_Min.x, Desc.vStartScaleXYZ_Max.x);
		m_vStartScaleRUL[i].y = fRandom(Desc.vStartScaleXYZ_Min.y, Desc.vStartScaleXYZ_Max.y);
		m_vStartScaleRUL[i].z = fRandom(Desc.vStartScaleXYZ_Min.z, Desc.vStartScaleXYZ_Max.z);

		m_vStartSpeedXYZ[i].x = fRandom(Desc.vStartSpeedXYZ_Min.x, Desc.vStartSpeedXYZ_Max.x);
		m_vStartSpeedXYZ[i].y = fRandom(Desc.vStartSpeedXYZ_Min.y, Desc.vStartSpeedXYZ_Max.y);
		m_vStartSpeedXYZ[i].z = fRandom(Desc.vStartSpeedXYZ_Min.z, Desc.vStartSpeedXYZ_Max.z);

		m_vStartRotateRUL[i].x = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.x, Desc.vRotateRUL_Max.x));
		m_vStartRotateRUL[i].y = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.y, Desc.vRotateRUL_Max.y));
		m_vStartRotateRUL[i].z = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.z, Desc.vRotateRUL_Max.z));

		m_fRotateAccelation[i].x = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.x, Desc.fRotateAccelation_Max.x));
		m_fRotateAccelation[i].y = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.y, Desc.fRotateAccelation_Max.y));
		m_fRotateAccelation[i].z = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.z, Desc.fRotateAccelation_Max.z));

		switch (Desc.iBillBoardMode)
		{
		case BILLBOARD_TYPE::NORMAL:
		{
			_vector vRadianAngles = { XMConvertToRadians(fRandom(0, 360)), XMConvertToRadians(fRandom(0, 360)), XMConvertToRadians(fRandom(0, 360)), 0.f };
			_vector	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
			_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);
			_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_vStartScaleRUL[i].x;
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_vStartScaleRUL[i].y;
			_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * m_vStartScaleRUL[i].z;
			XMStoreFloat4(&(*pOutInstance)[i].vRight, XMVector3TransformNormal(vRight, RotationMatrix));
			XMStoreFloat4(&(*pOutInstance)[i].vUp, XMVector3TransformNormal(vUp, RotationMatrix));
			XMStoreFloat4(&(*pOutInstance)[i].vLook, XMVector3TransformNormal(vLook, RotationMatrix));
		}
			break;
		case BILLBOARD_TYPE::BILLBOARD:
			break;
		case BILLBOARD_TYPE::DIRECTIONAL:
			break;
		default:
			break;
		}

		switch (Desc.iStartPointMode)
		{
		case STARTPOINT_TYPE::POINT:
			(*pOutInstance)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
			break;
		case STARTPOINT_TYPE::CUBE:
			(*pOutInstance)[i].vPosition = _float4(fRandom(Desc.vRangeXYZ_Min.x, Desc.vRangeXYZ_Max.x), fRandom(Desc.vRangeXYZ_Min.y, Desc.vRangeXYZ_Max.y), fRandom(Desc.vRangeXYZ_Min.z, Desc.vRangeXYZ_Max.z), 1.f);
			break;
		case STARTPOINT_TYPE::SPHERE:
		{
			_vector vPos = XMVectorSetW(XMVector3Normalize(XMVectorSet(fRandom(-1.f, 1.f), fRandom(-1.f, 1.f), fRandom(-1.f, 1.f), 0.f)) * fRandom(0.f, m_ParticleDesc.fRangeRadius), 1.f);
			XMStoreFloat4(&(*pOutInstance)[i].vPosition, vPos);
		}
		break;
		default:
			break;
		}

		switch (Desc.iMovingMode)
		{
		case MOVING_TYPE::SPEED:
			break;
		case MOVING_TYPE::ACCELATION:
		{
			// Accelation Mode.
			// Each Accelation의 방향은 시작 방향 단위벡터, 크기만 줄 것.
			_float fRandomEachAccelation = fRandom(Desc.fEachAccelation_MinMax.x, Desc.fEachAccelation_MinMax.y);
			_vector vEachAccelationDir = XMVector3Normalize(XMVectorSet(m_vStartSpeedXYZ[i].x, m_vStartSpeedXYZ[i].y, m_vStartSpeedXYZ[i].z, 0.f));
			m_fEachAccelationXYZ[i].x = XMVectorGetX(vEachAccelationDir) * fRandomEachAccelation;
			m_fEachAccelationXYZ[i].y = XMVectorGetY(vEachAccelationDir) * fRandomEachAccelation;
			m_fEachAccelationXYZ[i].z = XMVectorGetZ(vEachAccelationDir) * fRandomEachAccelation;
		}
		break;
		case MOVING_TYPE::POSITIONING:
		{
			if (m_ParticleDesc.bIsGatheringOnce) // Duration 동안 한번 모이고 끝남. --> 초기 속도로 쭉 고정.
			{
				// 속 = 거리 / 시간
				// 거리 == 초기 위치 - 타겟 위치.
				_vector vDisplacement = XMLoadFloat4(&(*pOutInstance)[i].vPosition) - XMLoadFloat3(&Desc.vTargetPosition);
				m_vStartSpeedXYZ[i].x = XMVectorGetX(vDisplacement) / Desc.fLifeTime.y;
				m_vStartSpeedXYZ[i].y = XMVectorGetY(vDisplacement) / Desc.fLifeTime.y;
				m_vStartSpeedXYZ[i].z = XMVectorGetZ(vDisplacement) / Desc.fLifeTime.y;
			}
		}
		break;
		default:
			break;
		}

	}
}

CVIBuffer_Rect_Instance* CVIBuffer_Rect_Instance::Create()
{
	CVIBuffer_Rect_Instance* pInstance = new CVIBuffer_Rect_Instance();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect_Instance::Clone(void* pArg)
{
	CVIBuffer_Rect_Instance* pInstance = new CVIBuffer_Rect_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
	Safe_Delete_Array(m_vStartScaleRUL);
	//Safe_Delete_Array(m_vCurrentScaleRUL);
	//Safe_Delete_Array(m_vCurrentSpeedXYZ);
	Safe_Delete_Array(m_vStartSpeedXYZ);
	Safe_Delete_Array(m_fEachAccelationXYZ);
	Safe_Delete_Array(m_vStartRotateRUL);
	//Safe_Delete_Array(m_vCurrentRotateSpeedRUL);
	Safe_Delete_Array(m_fRotateAccelation);
}
