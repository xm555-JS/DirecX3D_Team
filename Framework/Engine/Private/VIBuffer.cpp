#include "..\Public\VIBuffer.h"
#include "GameInstance.h"

CVIBuffer::CVIBuffer()
	: CComponent()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndicesStride(rhs.m_iIndicesStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_eToplogy(rhs.m_eToplogy)
	, m_pVerticesPos(rhs.m_pVerticesPos)
	, m_pIndices(rhs.m_pIndices)
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pContext)
		__debugbreak();

	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
	};

	_uint		iStrides[] = {
		m_iStride,
	};

	_uint		iOffsets[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eToplogy);

	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		__debugbreak();

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVB)))
		__debugbreak();

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		__debugbreak();

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pIB)))
		__debugbreak();

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVerticesPos);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
