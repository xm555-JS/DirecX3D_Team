#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Model_Instance final : public CVIBuffer
{

protected:
	CVIBuffer_Model_Instance();
	CVIBuffer_Model_Instance(const CVIBuffer_Model_Instance& rhs);
	virtual ~CVIBuffer_Model_Instance() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(0, 0, 0);
	}
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Set_SphereInsDesc(_matrix TargetMatrix);

public:
	virtual HRESULT Initialize_Prototype(_uint iMaterialIndex, VTXMODEL* pVertices, _float4* pVerticesPos, _uint iNumVertices, FACEINDICES32* pIndices, _uint iNumPrimitive, _fmatrix TransformMatrix, _uint iNumInstance);
	virtual HRESULT Initialize_Prototype(HANDLE hFile, DWORD* dwByte, class CModel* pModel, _uint iNumInstance, _fmatrix TransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

public:
	void Update(_float fTimeDelta , _float4 vTargetPos);
	HRESULT SetUp_BonesPtr(class CModel* pModel);
	void SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);

private:
	ID3D11Buffer*				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

	_float*						m_pInstanceSpeeds = nullptr;

	_float						m_fAccTime = 0.f;

private:
	class CModel*				m_pModel = nullptr;

private: /* 현재ㅑ 메시컨테이너에게 적용되어야할 머테리얼 인덱스*/
	char				m_szName[MAX_PATH] = "";

	_uint				m_iMaterialIndex = 0;

	_uint				m_iNumBones = 0;

	const aiMesh*		m_pAIMesh = nullptr;

	vector<class CHierarchyNode*>			m_Bones;
	vector<_uint>		m_BonesIndex;

	//쿼드트리 용 변수
//	FACEINDICES32*			m_pFaceIndices = nullptr;
//	class CQuadTree*		m_pQuadTree = nullptr;

private:
	_float4						m_vTranslation[MAX_PATH] = {};
	//SPHEREINSTANCEDESC			m_SphereInsDesc;

public:
	static CVIBuffer_Model_Instance* Create(_uint iMaterialIndex, VTXMODEL* pVertices, _float4* pVerticesPos, _uint iNumVertices, FACEINDICES32* pIndices, _uint iNumPrimitive, _fmatrix TransformMatrix, _uint iNumInstance = 1);
	static CVIBuffer_Model_Instance* Create(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END