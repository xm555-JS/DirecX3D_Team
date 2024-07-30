#include "VIBuffer_Trail.h"
#include "PipeLine.h"
#include "Easing_Utillity.h"

CVIBuffer_Trail::CVIBuffer_Trail()
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	NULLCHECK(pArg);
	memcpy(&m_stTrailDesc, pArg, sizeof(TRAIL_DESC));

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = m_stTrailDesc.iNumTrail * 2 + 2;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 0;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	FAILEDCHECK(__super::Create_VertexBuffer());

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = m_stTrailDesc.iNumTrail * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_stTrailDesc.iNumTrail * 2;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndicesStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;


	FACEINDICES16* pIndices = new FACEINDICES16[(m_iNumPrimitive)];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);
	//SetUp_ParticleDesc(m_ParticleDesc, &pInstance);


	for (_uint i = 0; i < m_iNumPrimitive; i += 2)
	{
		pIndices[i]._1 = i + 3;
		pIndices[i]._2 = i + 1;
		pIndices[i]._3 = i + 0;

		pIndices[i + 1]._1 = i + 2;
		pIndices[i + 1]._2 = i + 3;
		pIndices[i + 1]._3 = i + 0;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	FAILEDCHECK(__super::Create_IndexBuffer());

	Safe_Delete_Array(pIndices);
#pragma endregion


	//Setting_Index();
	return S_OK;
}

#if 0
HRESULT CVIBuffer_Trail::Initialize_Pooling(void* pArg)
{
	NULLCHECK(m_pContext);
	NULLCHECK(m_pVB);
	NULLCHECK(pArg);

	//memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLE_DESC));

	m_fTimeAcc = 0;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	//SetUp_ParticleDesc(m_ParticleDesc, (VTXTEX**)&SubResource.pData);

	m_pContext->Unmap(m_pVB, 0);

	return S_OK;
}
#endif // 0


HRESULT CVIBuffer_Trail::Render()
{
	NULLCHECK(m_pContext);

	__super::Render();

#if 0
	ID3D11Buffer* pVertexBuffers[] = {
m_pVB,
m_pVB
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
	//m_pContext->DrawIndexedInstanced(6, m_ParticleDesc.iNumInstance, 0, 0, 0);  
#endif // 0


	return S_OK;
}

void CVIBuffer_Trail::Update(_float fTimeDelta, TRAIL_DESC& _stTrailDesc)
{
	NULLCHECK(m_pContext);
	m_fTimeAcc += fTimeDelta;

	if (m_iVtxCount < m_iNumVertices)
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유효한 값이 담겨있는 형태로 얻어온다.(for문으로 돌때) */
		/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어온다. */
		m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		_vector vPos = { 0.f, 0.f, 0.f, 1.f };

		vPos = XMVector3TransformCoord(vPos, XMMatrixTranslationFromVector(XMLoadFloat4(&_stTrailDesc.vLocalSwordLow)));
		vPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&_stTrailDesc.matOwnerWorld));

		((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.x = XMVectorGetX(vPos);
		((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.y = XMVectorGetY(vPos);
		((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.z = XMVectorGetZ(vPos);

		vPos = { 0.f, 0.f, 0.f, 1.f };
		vPos = XMVector3TransformCoord(vPos, XMMatrixTranslationFromVector(XMLoadFloat4(&_stTrailDesc.vLocalSwordHigh)));
		vPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&_stTrailDesc.matOwnerWorld));

		((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.x = XMVectorGetX(vPos);
		((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.y = XMVectorGetY(vPos);
		((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.z = XMVectorGetZ(vPos);

		m_iVtxCount += 2;

		if (m_iVtxCount > m_iNumVertices)
			m_iVtxCount = m_iNumVertices;

		for (_uint i = 0; i < m_iVtxCount; i += 2)
		{
			((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 1.f };
			((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 0.f };
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pContext->Unmap(m_pVB, 0);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;
		m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		_uint iRemoveCount = m_iVtxCount / 4;
		m_iVtxCount -= iRemoveCount;
		for (_uint i = 0; i < m_iVtxCount; i += 2)
		{
			((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
			((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
		}

		m_pContext->Unmap(m_pVB, 0);
	}
}

#if 0
HRESULT CVIBuffer_Trail::Setting_Index(void)
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
#endif // 0


#if 0
void CVIBuffer_Trail::SetUp_ParticleDesc(PARTICLE_DESC Desc, VTXTEX** pOutInstance)
{
	for (_uint i = 0; i < Desc.iNumInstance; ++i)
	{
		m_vStartScaleRUL[i].x = fRandom(Desc.vStartScaleXYZ_Min.x, Desc.vStartScaleXYZ_Max.x);
		m_vStartScaleRUL[i].y = fRandom(Desc.vStartScaleXYZ_Min.y, Desc.vStartScaleXYZ_Max.y);
		m_vStartScaleRUL[i].z = fRandom(Desc.vStartScaleXYZ_Min.z, Desc.vStartScaleXYZ_Max.z);

		m_vStartSpeedXYZ[i].x = fRandom(Desc.vStartSpeedXYZ_Min.x, Desc.vStartSpeedXYZ_Max.x);
		m_vStartSpeedXYZ[i].y = fRandom(Desc.vStartSpeedXYZ_Min.y, Desc.vStartSpeedXYZ_Max.y);
		m_vStartSpeedXYZ[i].z = fRandom(Desc.vStartSpeedXYZ_Min.z, Desc.vStartSpeedXYZ_Max.z);

		m_vStartScaleRUL[i].x = fRandom(Desc.vStartScaleXYZ_Min.x, Desc.vStartScaleXYZ_Max.x);
		m_vStartScaleRUL[i].y = fRandom(Desc.vStartScaleXYZ_Min.y, Desc.vStartScaleXYZ_Max.y);
		m_vStartScaleRUL[i].z = fRandom(Desc.vStartScaleXYZ_Min.z, Desc.vStartScaleXYZ_Max.z);

		m_vStartRotateRUL[i].x = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.x, Desc.vRotateRUL_Max.x));
		m_vStartRotateRUL[i].y = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.y, Desc.vRotateRUL_Max.y));
		m_vStartRotateRUL[i].z = XMConvertToRadians(fRandom(Desc.vRotateRUL_Min.z, Desc.vRotateRUL_Max.z));

		m_fRotateAccelation[i].x = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.x, Desc.fRotateAccelation_Max.x));
		m_fRotateAccelation[i].y = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.y, Desc.fRotateAccelation_Max.y));
		m_fRotateAccelation[i].z = XMConvertToRadians(fRandom(Desc.fRotateAccelation_Min.z, Desc.fRotateAccelation_Max.z));

		switch (Desc.iBillBoardMode)
		{
		case BILLBOARD_TYPE::NORMAL:
			(*pOutInstance)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
			(*pOutInstance)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
			(*pOutInstance)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
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
#endif // 0


CVIBuffer_Trail* CVIBuffer_Trail::Create()
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}
