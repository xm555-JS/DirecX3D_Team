#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)
class CModel;

class ENGINE_DLL CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer();
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}
	
public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	_bool	Is_LoadFinish() { return m_bLoad_Finished; }

	const aiMesh* Get_AiMesh() { return m_pAIMesh; }

public:
	virtual HRESULT Initialize_Prototype(MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile);
	virtual HRESULT Initialize_Prototype(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix);
	virtual HRESULT Initialize(void* pArg, CMeshContainer* pPrototype);

public:
	HRESULT Load_Anim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel);
	HRESULT Load_NonAnim_Mesh(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix);

public:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix TransformMatrix, DWORD* dwByte, HANDLE hFile);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, class CModel* pModel, DWORD* dwByte, HANDLE hFile);
	HRESULT SetUp_BonesPtr(class CModel* pModel);
	void SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);

private: /* 현재ㅑ 메시컨테이너에게 적용되어야할 머테리얼 인덱스*/ 
	char				m_szName[MAX_PATH] = "";
	char				m_szBoneName[MAX_PATH] = "";

	_uint				m_iMaterialIndex = 0;

	_uint				m_iNumBones = 0;

	const aiMesh*		m_pAIMesh = nullptr;

	vector<class CHierarchyNode*>			m_Bones;
	vector<_uint>							m_BonesIndex;

	_bool				m_bLoad_Finished = false;

public:
	static CMeshContainer* Create(MODELTYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix TransformMatrix, DWORD* dwByte = nullptr, HANDLE hFile = nullptr);
	static CMeshContainer* Create(HANDLE hFile, DWORD* dwByte, CModel* pModel, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END