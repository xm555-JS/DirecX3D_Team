#include "..\Public\VIBuffer_Cloud_Instance.h"

CVIBuffer_Cloud_Instance::CVIBuffer_Cloud_Instance()
	: CVIBuffer()
{
}

CVIBuffer_Cloud_Instance::CVIBuffer_Cloud_Instance(const CVIBuffer_Cloud_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_pInstanceSpeeds(rhs.m_pInstanceSpeeds)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Cloud_Instance::Initialize_Prototype(_uint iNumInstance)
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

	VTXTEX*			pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.0f, 0.f);

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
	

#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumInstance = iNumInstance;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXINSTANCE*		pInstance = new VTXINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f ,0.f, 0.f);
		pInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vPosition = _float4(rand() % 10, 10.0f, rand() % 10, 1.f);
	}	

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	FAILEDCHECK(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance));

	Safe_Delete_Array(pInstance);

	/* �� �ν��Ͻ����� ����Ǿ���� ���ǵ带 ���� �ٸ��� �����Ѵ�. */
	m_pInstanceSpeeds = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pInstanceSpeeds[i] = rand() % 7 + 5.0f;
	}


#pragma endregion

#pragma region INDEXBUFFER
	

#pragma endregion
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

	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitive];
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
}

HRESULT CVIBuffer_Cloud_Instance::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Cloud_Instance::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer*			pVertexBuffers[] = {
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

	/* 6 : �ϳ��� ������ �׸������� ����ϴ� �ε����� ����. �׸�� ������.  */
	m_pContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;	
}

void CVIBuffer_Cloud_Instance::Update(_float fTimeDelta)
{
	if (nullptr == m_pContext ||
		nullptr == m_pVBInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource; 

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource����ü�� �޾ƿ� pData�� ������ ���� ����մ� ���·� ������. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource����ü�� �޾ƿ� pData�� ���� �ʱ�ȭ�� ���·� ������. */
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{

		// *(((VTXINSTANCE*)SubResource.pData) + i)
		((VTXINSTANCE*)SubResource.pData)[i].vPosition.y -= m_pInstanceSpeeds[i] * fTimeDelta;

		if (0.0f >= ((VTXINSTANCE*)SubResource.pData)[i].vPosition.y)
			((VTXINSTANCE*)SubResource.pData)[i].vPosition.y = 10.f;

	}

	m_pContext->Unmap(m_pVBInstance, 0);

}

CVIBuffer_Cloud_Instance * CVIBuffer_Cloud_Instance::Create(_uint iNumInstance)
{
	CVIBuffer_Cloud_Instance*		pInstance = new CVIBuffer_Cloud_Instance();

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Cloud_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cloud_Instance::Clone(void * pArg)
{
	CVIBuffer_Cloud_Instance*		pInstance = new CVIBuffer_Cloud_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Cloud_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cloud_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pInstanceSpeeds);

	Safe_Release(m_pVBInstance);
}
