#include "..\Public\Mesh_Instance.h"
#include "PipeLine.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "GameInstance.h"


CMesh_Instance::CMesh_Instance()
	: CVIBuffer()
{
}

CMesh_Instance::CMesh_Instance(const CMesh_Instance& rhs)
	: CVIBuffer(rhs)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_BonesIndex(rhs.m_BonesIndex)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CMesh_Instance::Initialize_Prototype(MODELTYPE eType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile)
{
	m_pAIMesh = pAIMesh;
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	strcpy_s(m_szName, pAIMesh->mName.data);

	WriteFile(hFile, &m_iMaterialIndex, sizeof(_uint), dwByte, nullptr);

	DWORD iLen = (DWORD)strlen(m_szName) + 1;
	WriteFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	WriteFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

#pragma region VERTEXBUFFER

	HRESULT		hr = 0;

	if (TYPE_NONANIM_INSTANCE == eType)
		hr = Ready_VertexBuffer_NonAnim(pAIMesh, TransformMatrix, dwByte, hFile);
	else
		hr = Ready_VertexBuffer_Anim(pAIMesh, pModel, dwByte, hFile);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._3 = pAIMesh->mFaces[i].mIndices[2];
	}

	WriteFile(hFile, &m_iNumPrimitive, sizeof(_uint), dwByte, nullptr);
	WriteFile(hFile, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive, dwByte, nullptr);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh_Instance::Initialize_Prototype(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformationMatrix)
{
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), dwByte, nullptr);
	DWORD iLen;
	ReadFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	ReadFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

#pragma region VERTEXBUFFER

	HRESULT		hr = 0;

	if (TYPE_NONANIM_INSTANCE == pModel->Get_ModelType())
		hr = Load_NonAnim_Mesh(hFile, dwByte, pModel, TransformationMatrix);
	else
		hr = Load_Anim_Mesh(hFile, dwByte, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion

	m_iIndicesStride = sizeof(FACEINDICES32);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), dwByte, nullptr);
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	ReadFile(hFile, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive, dwByte, nullptr);
	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh_Instance::Initialize(void* pArg)
{
	if (nullptr == m_pContext)
		return E_FAIL;

	if (nullptr != pArg)
		m_iInstanceDesc = *((_uint*)pArg);

#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumInstance = m_iInstanceDesc;			//pArg 구조체 만들어서 인스터스 갯수등등 적용할것

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXINSTANCE* pInstance = new VTXINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

#if 0
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	ZeroMemory(&SubResource_Origin, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_READ, 0, &SubResource);

	(VTXINSTANCE*)SubResource_Origin = (VTXINSTANCE*)SubResource[i];
	m_pContext->Unmap(m_pVBInstance, 0);
#endif // 0


#pragma endregion

	return S_OK;
}

void CMesh_Instance::Set_InsDesc(list<_matrix> Matrixs)
{
	m_listInstanceDesc = Matrixs;
	if (nullptr == m_pContext ||
		nullptr == m_pVBInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_uint i = 0;
	for (auto& iter : Matrixs)
	{
		if (i > m_iNumInstance)
			break;

		XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vRight, iter.r[0]);
		XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vUp, iter.r[1]);
		XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vLook, iter.r[2]);
		XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vPosition, iter.r[3]);
		++i;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CMesh_Instance::Render()
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
	m_pContext->DrawIndexedInstanced(m_iNumIndices, m_iNumInstance_Render, 0, 0, 0);

	return S_OK;
}

HRESULT CMesh_Instance::Load_Anim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel)
{
	m_iStride = sizeof(VTXANIM);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), dwByte, nullptr);
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIM* pVertices = new VTXANIM[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIM) * m_iNumVertices);
	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	ReadFile(hFile, pVertices, sizeof(VTXANIM) * m_iNumVertices, dwByte, nullptr);
	ReadFile(hFile, m_pVerticesPos, sizeof(_float3) * m_iNumVertices, dwByte, nullptr);

	ReadFile(hFile, &m_iNumBones, sizeof(_uint), dwByte, nullptr);

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_uint iBonesIndex;
		ReadFile(hFile, &iBonesIndex, sizeof(_uint), dwByte, nullptr);

		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(iBonesIndex);

		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_BonesIndex.push_back(iBonesIndex);

		m_Bones.push_back(pHierarchyNode);

		//Safe_AddRef(pHierarchyNode);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CMesh_Instance::Load_NonAnim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix)
{
	m_iStride = sizeof(VTXMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), dwByte, nullptr);
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);
	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	ReadFile(hFile, pVertices, sizeof(VTXMODEL) * m_iNumVertices, dwByte, nullptr);
	ReadFile(hFile, m_pVerticesPos, sizeof(_float3) * m_iNumVertices, dwByte, nullptr);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), TransformMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), TransformMatrix)));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CMesh_Instance::Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), dwByte, nullptr);

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));

		m_pVerticesPos[i] = _float3(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z);

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	WriteFile(hFile, pVertices, sizeof(VTXMODEL) * m_iNumVertices, dwByte, nullptr);
	WriteFile(hFile, m_pVerticesPos, sizeof(_float3) * m_iNumVertices, dwByte, nullptr);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), TransformMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), TransformMatrix)));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh_Instance::Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, CModel* pModel, DWORD* dwByte, HANDLE hFile)
{
	m_iStride = sizeof(VTXANIM);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIM* pVertices = new VTXANIM[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIM) * m_iNumVertices);

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));

		m_pVerticesPos[i] = _float3(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z);

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	m_iNumBones = pAIMesh->mNumBones;

	_uint* iBones = new _uint[m_iNumBones];

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		_float4x4	OffSetMatrix;

		memcpy(&OffSetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		CHierarchyNode* pHierarchyNode = pModel->Find_HierarcyNode(pAIBone->mName.data);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		iBones[i] = pModel->Find_HierarcyNodeIndex(pAIBone->mName.data);

		pHierarchyNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffSetMatrix)));

		m_Bones.push_back(pHierarchyNode);
		m_BonesIndex.push_back(iBones[i]);
		//Safe_AddRef(pHierarchyNode);

		/* pAIBone->mNumWeights : 이 뼈는 몇개의 정점에 영향릉 주는지 */
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : 그 중에 j번째 정점의 인덱스는 뭐였는지?  */
			/* pAIBone->mWeights[j].mWeight : j번째 정점에게 적용해야할 가중치. */

			if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		CHierarchyNode* pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
		if (nullptr == pHierarchyNode)
			return E_FAIL;
		iBones[0] = pModel->Find_HierarcyNodeIndex(m_szName);

		m_Bones.push_back(pHierarchyNode);
		m_BonesIndex.push_back(iBones[0]);

		//Safe_AddRef(pHierarchyNode);
	}

	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), dwByte, nullptr);
	WriteFile(hFile, pVertices, sizeof(VTXANIM) * m_iNumVertices, dwByte, nullptr);
	WriteFile(hFile, m_pVerticesPos, sizeof(_float3) * m_iNumVertices, dwByte, nullptr);
	WriteFile(hFile, &m_iNumBones, sizeof(_uint), dwByte, nullptr);
	WriteFile(hFile, iBones, sizeof(_uint) * m_iNumBones, dwByte, nullptr);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(iBones);
	Safe_Delete_Array(pVertices);

	return S_OK;
}

void CMesh_Instance::Update(_bool bUseFrustum/*_float fTimeDelta, _float4 vTargetPos*/)
{
	if (nullptr == m_pContext ||
		nullptr == m_pVBInstance)
		return;

	//m_fAccTime += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	m_iNumInstance_Render = 0;

	_uint iIndex = 0;
	for (auto& iter : m_listInstanceDesc)
	{
		if (iIndex > m_iNumInstance)
			break;

		_float fLargestVal = max(max(
			XMVectorGetX(XMVector3Length(iter.r[0])),
			XMVectorGetX(XMVector3Length(iter.r[1]))),
			XMVectorGetX(XMVector3Length(iter.r[2])));

		if (bUseFrustum)
		{
			if (4.f <= fLargestVal || GAMEINSTANCE->isIn_Frustum_InWorldSpace(iter.r[3], fLargestVal * 50.f))
			{
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vRight, iter.r[0]);
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vUp, iter.r[1]);
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vLook, iter.r[2]);
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vPosition, iter.r[3]);
				++m_iNumInstance_Render;
			}
		}
		else
		{
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vRight, iter.r[0]);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vUp, iter.r[1]);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vLook, iter.r[2]);
			XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[m_iNumInstance_Render].vPosition, iter.r[3]);
			++m_iNumInstance_Render;
		}

		++iIndex;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CMesh_Instance::SetUp_BonesPtr(CModel* pModel)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(m_BonesIndex[i]);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_Bones.push_back(pHierarchyNode);

		Safe_AddRef(pHierarchyNode);
	}

	return S_OK;
}

void CMesh_Instance::SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix)
{
	if (m_iNumBones > 256)
		return;

	_uint		iIndex = 0;

	for (auto& pBone : m_Bones)
	{
		XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(pBone->Get_OffsetMatrix() * pBone->Get_CombinedMatrix() * TransformationMatrix));
	}
}

CMesh_Instance* CMesh_Instance::Create(MODELTYPE eType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile)
{
	CMesh_Instance* pInstance = new CMesh_Instance();

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, pModel, TransformMatrix, dwByte, hFile)))
	{
		MSG_BOX("Failed to Created : CMesh_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh_Instance* CMesh_Instance::Create(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix)
{
	CMesh_Instance* pInstance = new CMesh_Instance();

	if (FAILED(pInstance->Initialize_Prototype(hFile, dwByte, pModel, TransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh_Instance::Clone(void* pArg)
{
	CMesh_Instance* pInstance = new CMesh_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

	//for (auto& pHierarchyNode : m_Bones)
	//	Safe_Release(pHierarchyNode);

	m_Bones.clear();

	//if (false == m_isCloned)
	//	Safe_Delete_Array(m_pFaceIndices);

	//Safe_Release(m_pQuadTree);
}
