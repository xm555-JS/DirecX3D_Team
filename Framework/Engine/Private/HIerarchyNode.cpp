#include "..\Public\HIerarchyNode.h"

#include "Model.h"

CHierarchyNode::CHierarchyNode()
{
}

void CHierarchyNode::SetParent(CModel* pModel)
{
	m_pModel = pModel;

	if (-1 == m_iParentIndex)
		return;

	m_pParent = pModel->Get_HierarchyNode(m_iParentIndex);
	//Safe_AddRef(m_pParent);
}

HRESULT CHierarchyNode::Initialize(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix, _uint* iIndex)
{
	m_iDepth = iDepth;

	m_pParent = pParent;

	m_iCurIndex = *iIndex;
	
	if (nullptr != m_pParent)
		m_iParentIndex = m_pParent->Get_CurrentIndex();

	strcpy_s(m_szName, pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_TransformMatrix = TransformMatrix;	

	return S_OK;
}

HRESULT CHierarchyNode::Initialize(HANDLE hFile, DWORD* dwByte, _float4x4 TransformMatrix)
{
	ReadFile(hFile, &m_iDepth, sizeof(_uint), dwByte, nullptr);
	DWORD iLen;
	ReadFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	ReadFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

	ReadFile(hFile, &m_TransformationMatrix, sizeof(_float4x4), dwByte, nullptr);
	ReadFile(hFile, &m_OffsetMatrix, sizeof(_float4x4), dwByte, nullptr);
	ReadFile(hFile, &m_CombinedTransformationMatrix, sizeof(_float4x4), dwByte, nullptr);

	ReadFile(hFile, &m_iCurIndex, sizeof(_int), dwByte, nullptr);
	ReadFile(hFile, &m_iParentIndex, sizeof(_int), dwByte, nullptr);

	m_TransformMatrix = TransformMatrix;

	return S_OK;
}

HRESULT CHierarchyNode::Initialize(CHierarchyNode* pPrototype)
{
	m_iDepth = pPrototype->m_iDepth;

	strcpy_s(m_szName, pPrototype->m_szName);
	memcpy(&m_OffsetMatrix, &pPrototype->m_OffsetMatrix, sizeof(_float4x4));
	memcpy(&m_TransformationMatrix, &pPrototype->m_TransformationMatrix, sizeof(_float4x4));
	memcpy(&m_CombinedTransformationMatrix, &pPrototype->m_CombinedTransformationMatrix, sizeof(_float4x4));
	memcpy(&m_TransformMatrix, &pPrototype->m_TransformMatrix, sizeof(_float4x4));
	m_iCurIndex = pPrototype->m_iCurIndex;
	m_iParentIndex = pPrototype->m_iParentIndex;

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix(string strRootBoneName, _float4x4* pOut)
{
	//"Bip001"
	m_bRootNode = false;
	if (!strcmp(strRootBoneName.c_str(), m_szName))
	{
		m_bRootNode = true;
		//XMStoreFloat4x4(&m_TransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * m_pModel->Get_RotateOffset());;
	}

	if (nullptr == m_pParent)
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
		//XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * m_pModel->Get_RotateOffset());;
	}



	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * m_pParent->Get_CombinedMatrix());
}

CHierarchyNode * CHierarchyNode::Create(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix, _uint* iIndex)
{
	CHierarchyNode*		pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pAINode, pParent, iDepth, TransformMatrix, iIndex)))
	{
		MSG_BOX("Failed to Created : CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode* CHierarchyNode::Create(HANDLE hFile, DWORD* dwByte, _float4x4 TransformMatrix)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(hFile, dwByte, TransformMatrix)))
	{
		MSG_BOX("Failed to Created : CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode * CHierarchyNode::Clone()
{
	CHierarchyNode* pInstance = new CHierarchyNode(*this);

	if (FAILED(pInstance->Initialize(this)))
	{
		MSG_BOX("Failed to Cloned : CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	//Safe_Release(m_pParent);
}

