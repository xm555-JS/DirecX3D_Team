#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;
public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedMatrix() const {

		if (m_bRootNode)
		{
			_matrix Temp = XMLoadFloat4x4(&m_CombinedTransformationMatrix);
			Temp.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);


			return Temp;
		}
		else
			return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	_matrix Get_RealCombinedMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}


	_matrix Get_TransformMatrix() const {
		return XMLoadFloat4x4(&m_TransformMatrix);
	}

	_matrix Get_TransformationMatrix() const {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}

	_int	Get_CurrentIndex() { return m_iCurIndex; }
	_int	Get_ParentsIndex() { return m_iParentIndex; }

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void SetParent(class CModel* pModel);
	void SetParentIndex(_uint iIndex) { m_iParentIndex = iIndex; }

public:
	HRESULT Initialize(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix, _uint* iIndex);
	HRESULT Initialize(HANDLE hFile, DWORD* dwByte, _float4x4 TransformMatrix);
	HRESULT Initialize(CHierarchyNode* pPrototype);
	void Update_CombinedTransformationMatrix(string strRootBoneName = "", _float4x4* pOut = nullptr);

private:
	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";

	_bool			m_bRootNode = false;

	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;
	_float4x4		m_TransformMatrix;

	CHierarchyNode*	m_pParent = nullptr;
	_int m_iCurIndex = -1;
	_int m_iParentIndex = -1;
	
	CModel*			m_pModel = nullptr;

public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix, _uint* iIndex);
	static CHierarchyNode* Create(HANDLE hFile, DWORD* dwByte, _float4x4 TransformMatrix);
	CHierarchyNode* Clone();
	virtual void Free() override;

};

END