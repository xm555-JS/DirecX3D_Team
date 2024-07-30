#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel();
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint								Get_NumMeshContainers() const { return m_iNumMeshContainers; }
	vector<class CMeshContainer*>*		Get_MeshContainers() { return &m_MeshContainers; }

	class CHierarchyNode*				Get_HierarchyNode(_uint iIndex) { return m_HierarchyNodes[iIndex]; }
	vector<class CHierarchyNode*>		Get_Hierarchylist() { return m_HierarchyNodes; }

	vector<class CAnimation*>			Get_Animation() { return m_Animations; }
	_bool								Get_AnimFinished();
	_vector								Get_AnimDiff() { return XMLoadFloat3(&m_vAnimPosDiff); }
	_uint								Get_PreAnimationIndex() { return m_iPreAnimationIndex; }

	_uint								Get_AnimGroup() { return m_iAnimGroup; }
	_uint								Get_AnimState() { return m_iAnimState; }

	MODELTYPE							Get_ModelType() { return m_eModelType; }
	_float4x4							Get_TransformMatrix() { return m_TransformMatrix; }
	//_matrix								Get_RotateOffset() { return XMLoadFloat4x4(&m_RotateOffset); }
	
	_uint								Get_Easingtype() { return m_iEasingtype; }
	
	_bool								Get_ChangeAnimation() { return m_bChangeAnimation; }
	_bool								Get_Transform_Turn_End() { return m_bTransform_Turn_End; }

public:
	void				Set_InsDesc(list<_matrix> Matrixs);
	void				Set_ModelEasingRatio(_float fRatio) { m_fTimeRatio = fRatio; }
	void				Set_RootEasing(_bool Easing) { m_bRootEasing = Easing; }
	void				Set_EasingTest(_bool Easing) { m_bEasingTest = Easing; }

	void				Set_CurrentAnimation(_uint iAnimIndex) { m_iCurrentAnimationIndex = iAnimIndex; }
	void				Set_PreAnimationIndex(_uint iAnimIndex) { m_iPreAnimationIndex = iAnimIndex; }
	HRESULT				Set_AnimMotion(string strRootBoneName = "", _float4x4* pOut = nullptr);
	HRESULT				Set_AnimationGroup(_uint iAnimGroup, _uint iAnimState);
	HRESULT				Set_AnimationGroup(_uint iAnimGroup);

	void				Set_RotateOffset(_matrix RotateOffset) { XMStoreFloat4x4(&m_RotateOffset, RotateOffset); }

	void				Set_AnimationEnd(_uint iKeyframe) { m_iPreKeyFrame = iKeyframe; }
	//void				Set_AnimationRotateMatrix(_matrix RotateMat, _uint iIdx) { XMStoreFloat4x4(&m_pAnimOffsetMat[iIdx], RotateMat); }


public:
	class CHierarchyNode* Find_HierarcyNode(const char* pBoneName);
	//class CHierarchyNode* Find_HierarcyNodeIndex(const char* pBoneName);
	_uint Find_HierarcyNodeIndex(const char* pBoneName);

public:
	HRESULT Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix, _uint iAnimGroup, _uint iAnimState);
	HRESULT Initialize(void* pArg, CModel* pPrototype);
	HRESULT Play_Animation(_float fTimeDelta, string strRootBoneName = "", _float4x4* pOut = nullptr);
	HRESULT Play_Animation_Easing(_float fTimeDelta, string strRootBoneName = "", _float4x4* pOut = nullptr, _uint iEasingtype = 0, class CTransform* = nullptr);

	
	//HRESULT	Reset_KeyFrame();
	HRESULT Render(_uint iMeshContainerIndex, class CShader* pShader, _uint iPassIndex = 0, const char* pConstantBoneName = nullptr, _bool bUseFrustum = true);

public:
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, _uint iMeshContainerIndex, aiTextureType eType);

private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	MODELTYPE					m_eModelType = TYPE_END;
	_float4x4					m_TransformMatrix;
	string						m_strBoneName = "";
	_bool						m_bRootEasing = false;

	_bool						m_bEasingTest = false;
	_uint						m_iEasingtype = 0;
	_uint						m_iPreKeyFrame = 0;

private:
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	_uint									m_iNumMeshContainers = 0;

private:
	vector<class CMesh_Instance*>			m_MeshInstanceContainers;
	typedef vector<class CMesh_Instance*>	MESHINSTANCECONTAINERS;

private:
	vector<MODEL_MATERIAL>					m_Materials;
	typedef vector<MODEL_MATERIAL>			MATERIALS;
	_uint									m_iNumMaterials = 0;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;
	_uint									m_iNumHierarchyNode = 0;

	vector<HIERARCHYNODEDATADESC>			m_HierarchyNodesData;

private:
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iPreAnimationIndex = 0;
	_float									m_fTimeAcc = 0.f;
	_float									m_fTimeRatio = 0.1f;

	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

	ANIMATIONS								m_PreAnimations;
	_bool									m_bAnimGroupChange = false;

	class CAnimation_Manager*				m_pAnimManager = nullptr;


private:
	_float3		m_vCurTargetPos;
	_float3		m_vAnimPosDiff;
	_float3		m_vPreTargetPos;

private:
	_uint		m_iAnimGroup = 0;
	_uint		m_iAnimState = 0;
	_uint		m_iPreAnimGroup = 0;
	_uint		m_iPreAnimState = 0;

private:
	//_float4x4*	m_pAnimOffsetMat = nullptr;
	_float4x4			m_RotateOffset;
	_float4				m_vOriDestRotation;
	class CChannel*		m_pTargetChannel = nullptr;
	KEYFRAME			m_TargetKeyframe;
	_bool				m_bChangeAnimation = false;
	_bool				m_bTransform_Turn_End = false;

private:
	HRESULT	Check_File(const char* pModelFilePath, const char* pModelFileName);

	HRESULT Create_MeshContainers();
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _int iParentIndex, _uint* iIndex, _uint iDepth);
	HRESULT Create_Animations();

	HRESULT Save_HierarchyNodes(const char* pModelFilePath);
	HRESULT	Save_MeshContainers(const char* pFullFilePath);
	HRESULT	Save_Materials(const char* pModelFilePath, const char* pModelFileName);
	HRESULT	Save_Animation(const char* pModelFilePath, _uint iAnimGroup);

	HRESULT	Load_HierarchyNodes(const char* pFullFilePath = nullptr);
	HRESULT	Load_MeshContainers(const char* pFullFilePath);
	HRESULT	Load_Materials(const char* pFullFilePath);
	HRESULT	Load_Animation(const char* pModelFilePath, _uint iAnimGroup);

public:
	static CModel* Create(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix, _uint iAnimGroup = 0, _uint iAnimState = 0);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END