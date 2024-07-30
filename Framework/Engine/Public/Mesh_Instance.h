#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CModel;

class ENGINE_DLL CMesh_Instance final : public CVIBuffer
{

protected:
	CMesh_Instance();
	CMesh_Instance(const CMesh_Instance& rhs);
	virtual ~CMesh_Instance() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(0, 0, 0);
	}
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Set_InsDesc(list<_matrix> Matrixs);

public:
	virtual HRESULT Initialize_Prototype(MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile);
	virtual HRESULT Initialize_Prototype(HANDLE hFile, DWORD* dwByte, class CModel* pModelx, _fmatrix TransformationMatrix);

	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

public:
	HRESULT Load_Anim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel);
	HRESULT Load_NonAnim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix);

public:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, class CModel* pModel, DWORD* dwByte, HANDLE hFile);


public:
	void Update(_bool bUseFrustum = true/*_float fTimeDelta , _float4 vTargetPos*/);
	HRESULT SetUp_BonesPtr(class CModel* pModel);
	void SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);

private:
	ID3D11Buffer*				m_pVBInstance = nullptr;
	ID3D11Buffer*				m_pVBInstance_Render = nullptr;
	D3D11_MAPPED_SUBRESOURCE	SubResource_Origin;
	list<_matrix>				m_listInstanceDesc;

	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;
	_uint						m_iNumInstance_Render = 0;

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

private:
	_uint			m_iInstanceDesc;

public:
	static CMesh_Instance* Create(MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte = nullptr, HANDLE hFile = nullptr);
	static CMesh_Instance* Create(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END