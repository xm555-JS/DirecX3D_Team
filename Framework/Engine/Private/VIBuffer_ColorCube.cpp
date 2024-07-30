#include "..\Public\VIBuffer_ColorCube.h"
#include "GameInstance.h"

CVIBuffer_ColorCube::CVIBuffer_ColorCube()
{
}

CVIBuffer_ColorCube::CVIBuffer_ColorCube(const CVIBuffer_ColorCube & Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_ColorCube::Initialize_Prototype()
{

	//m_iStride = sizeof(VTXCOLOR);
	//m_iNumVertices = 8;
	//m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE; /* | D3DFVF_TEXCOORDSIZE2(0) */
	//m_iNumPrimitive = 12;

	//									/* 정점배열이 할당된다. */
	//if (FAILED(__super::Create_VertexBuffer()))
	//	return E_FAIL;

	//VTXCOLOR*			pVertices = nullptr;

	///*void**		pData = (void**)&pVertices;*/

	//m_pVB->Lock(0, m_iStride * m_iNumVertices, (void**)&pVertices, 0);

	//pVertices[0].vPosition = m_vPos + _float3(-m_vScale.x, m_vScale.y, -m_vScale.z);
	//pVertices[0].dwColor = m_dwColor;

	//pVertices[1].vPosition = m_vPos + _float3(m_vScale.x, m_vScale.y, -m_vScale.z);
	//pVertices[1].dwColor = m_dwColor;

	//pVertices[2].vPosition = m_vPos + _float3(m_vScale.x, -m_vScale.y, -m_vScale.z);
	//pVertices[2].dwColor = m_dwColor;

	//pVertices[3].vPosition = m_vPos + _float3(-m_vScale.x, -m_vScale.y, -m_vScale.z);
	//pVertices[3].dwColor = m_dwColor;

	//pVertices[4].vPosition = m_vPos + _float3(-m_vScale.x, m_vScale.y, m_vScale.z);
	//pVertices[4].dwColor = m_dwColor;

	//pVertices[5].vPosition = m_vPos + _float3(m_vScale.x, m_vScale.y, m_vScale.z);
	//pVertices[5].dwColor = m_dwColor;

	//pVertices[6].vPosition = m_vPos + _float3(m_vScale.x, -m_vScale.y, m_vScale.z);
	//pVertices[6].dwColor = m_dwColor;

	//pVertices[7].vPosition = m_vPos + _float3(-m_vScale.x, -m_vScale.y, m_vScale.z);
	//pVertices[7].dwColor = m_dwColor;

	//m_pVB->Unlock();

	//m_iIndicesSizePerPrimitive = sizeof(FACEINDICES16);
	//m_eFormat = D3DFMT_INDEX16;
	//m_ePrimitiveType = D3DPT_TRIANGLELIST;

	//if (FAILED(__super::Create_IndexBuffer()))
	//	return E_FAIL;

	//FACEINDICES16*		pIndices = nullptr;

	//m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	//// front
	//pIndices[0] = { 0, 1, 2 };
	//pIndices[1] = { 0, 2, 3 };

	//// back
	//pIndices[2] = { 5, 4, 7 };
	//pIndices[3] = { 5, 7, 6 };

	//// up
	//pIndices[4] = { 0, 5, 1 };
	//pIndices[5] = { 0, 4, 5 };

	//// down
	//pIndices[6] = { 3, 2, 7 };
	//pIndices[7] = { 2, 6, 7 };

	//// right
	//pIndices[8] = { 1, 5, 2 };
	//pIndices[9] = { 5, 6, 2 };

	//// left
	//pIndices[10] = { 4, 0, 3 };
	//pIndices[11] = { 4, 3, 7 };

	////pIndices[0]._1 = 0;
	////pIndices[0]._2 = 1;
	////pIndices[0]._3 = 2;

	////pIndices[1]._1 = 0;
	////pIndices[1]._2 = 2;
	////pIndices[1]._3 = 3;


	//m_pIB->Unlock();


	//SHADER->Set_VB();			// 쉐이더 적용할 VB
	//SHADER->Set_Stride();		// Vertex 하나의 바이트 크기
	//SHADER->Set_FVF();			// 사용할 정점 설정
	//SHADER->Set_Effect();		// 사용할 쉐이더 포인터
	//SHADER->Set_Technique();	// 사용할 쉐이더 테크닉

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_ColorCubeDesc, pArg, sizeof(COLORCUBEDESC));

	m_vScale = m_ColorCubeDesc.vScale;
	m_dwColor = m_ColorCubeDesc.dwColor;
	m_vPos = m_ColorCubeDesc.vPos;


	//m_dwColor = D3DCOLOR_ARGB(150, 0, 0, 0);
	//m_vScale = { 0.11f, 0.5f, 0.60f };
	//m_vPos = { 0.13f, 0.f, 0.67f };


	m_iStride = sizeof(VTXCOLOR);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE; /* | D3DFVF_TEXCOORDSIZE2(0) */
	m_iNumPrimitive = 12;

	/* 정점배열이 할당된다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCOLOR*			pVertices = nullptr;

	/*void**		pData = (void**)&pVertices;*/

	m_pVB->Lock(0, m_iStride * m_iNumVertices, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vPos + _float3(-m_vScale.x, m_vScale.y, -m_vScale.z);
	pVertices[0].dwColor = m_dwColor;

	pVertices[1].vPosition = m_vPos + _float3(m_vScale.x, m_vScale.y, -m_vScale.z);
	pVertices[1].dwColor = m_dwColor;

	pVertices[2].vPosition = m_vPos + _float3(m_vScale.x, -m_vScale.y, -m_vScale.z);
	pVertices[2].dwColor = m_dwColor;

	pVertices[3].vPosition = m_vPos + _float3(-m_vScale.x, -m_vScale.y, -m_vScale.z);
	pVertices[3].dwColor = m_dwColor;

	pVertices[4].vPosition = m_vPos + _float3(-m_vScale.x, m_vScale.y, m_vScale.z);
	pVertices[4].dwColor = m_dwColor;

	pVertices[5].vPosition = m_vPos + _float3(m_vScale.x, m_vScale.y, m_vScale.z);
	pVertices[5].dwColor = m_dwColor;

	pVertices[6].vPosition = m_vPos + _float3(m_vScale.x, -m_vScale.y, m_vScale.z);
	pVertices[6].dwColor = m_dwColor;

	pVertices[7].vPosition = m_vPos + _float3(-m_vScale.x, -m_vScale.y, m_vScale.z);
	pVertices[7].dwColor = m_dwColor;

	m_pVB->Unlock();

	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES16);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	// front
	pIndices[0] = { 0, 1, 2 };
	pIndices[1] = { 0, 2, 3 };

	// back
	pIndices[2] = { 5, 4, 7 };
	pIndices[3] = { 5, 7, 6 };

	// up
	pIndices[4] = { 0, 5, 1 };
	pIndices[5] = { 0, 4, 5 };

	// down
	pIndices[6] = { 3, 2, 7 };
	pIndices[7] = { 2, 6, 7 };

	// right
	pIndices[8] = { 1, 5, 2 };
	pIndices[9] = { 5, 6, 2 };

	// left
	pIndices[10] = { 4, 0, 3 };
	pIndices[11] = { 4, 3, 7 };

	//pIndices[0]._1 = 0;
	//pIndices[0]._2 = 1;
	//pIndices[0]._3 = 2;

	//pIndices[1]._1 = 0;
	//pIndices[1]._2 = 2;
	//pIndices[1]._3 = 3;


	m_pIB->Unlock();

	return S_OK;
}

CVIBuffer_ColorCube * CVIBuffer_ColorCube::Create()
{
	CVIBuffer_ColorCube*		pInstance = new CVIBuffer_ColorCube();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_ColorCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_ColorCube::Clone(void * pArg)
{
	CVIBuffer_ColorCube*		pInstance = new CVIBuffer_ColorCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_ColorCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_ColorCube::Free()
{
	__super::Free();
}
