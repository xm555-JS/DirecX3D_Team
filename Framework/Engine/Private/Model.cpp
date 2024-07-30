#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Mesh_Instance.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Channel.h"
#include "Animation_Manager.h"

#include "Texture.h"
#include "Shader.h"

#include "Easing_Utillity.h"

#include "Transform.h"


CModel::CModel()
	: CComponent()
{

}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_TransformMatrix(rhs.m_TransformMatrix)
	, m_eModelType(rhs.m_eModelType)
{
	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_AddRef(pTexture);
	}
}

_bool CModel::Get_AnimFinished()
{
	return m_Animations[m_iCurrentAnimationIndex]->Get_IsFinished();
}

CHierarchyNode* CModel::Find_HierarcyNode(const char* pBoneName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pBoneName, pNode->Get_Name());
		});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;
}

_uint CModel::Find_HierarcyNodeIndex(const char* pBoneName)
{
	for (_uint i = 0; i < m_HierarchyNodesData.size(); ++i)
	{
		if (!strcmp(m_HierarchyNodesData[i].szName, pBoneName))
		{
			return i;
		}
	}

	return -1;
}

void CModel::Set_InsDesc(list<_matrix> Matrixs)
{
	for (auto& iter : m_MeshInstanceContainers)
	{
		iter->Set_InsDesc(Matrixs);
	}
}

HRESULT CModel::Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix, _uint iAnimGroup, _uint iAnimState)
{
	m_eModelType = eType;

	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	char	szModelFilePath[MAX_PATH] = "";
	char	szModelFileName[MAX_PATH] = "";

	strcpy_s(szModelFilePath, pModelFilePath);
	strcpy_s(szModelFileName, pModelFileName);

	char	szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	if (FAILED(Check_File(szModelFilePath, szModelFileName)))
	{
		if (TYPE_NONANIM == m_eModelType
			|| TYPE_NONANIM_INSTANCE == m_eModelType)
			m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
		else
			m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		string strFilePath(szModelFilePath);
		if (strFilePath.substr(strFilePath.length() - 7).compare("Effect/")) // Effect 관련파일이 아닌것만 Tex저장.
		{
			FAILEDCHECK(Save_Materials(szModelFilePath, szModelFileName));

			if (TYPE_NONANIM != m_eModelType
				&& TYPE_NONANIM_INSTANCE != m_eModelType)
			{
				_uint iHierarchyNodeIndex = 0;

				FAILEDCHECK(Create_HierarchyNodes(m_pAIScene->mRootNode, nullptr, -1, &iHierarchyNodeIndex, 0));

				sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
					{
						return pSour->Get_Depth() < pDest->Get_Depth();
					});

				sort(m_HierarchyNodesData.begin(), m_HierarchyNodesData.end(), [](HIERARCHYNODEDATADESC tSour, HIERARCHYNODEDATADESC tDest)
					{
						return tSour.iDepth < tDest.iDepth;
					});
			}
		}

		FAILEDCHECK(Save_MeshContainers(szFullPath));

		if (TYPE_NONANIM != m_eModelType
			&& TYPE_NONANIM_INSTANCE != m_eModelType)
		{
			FAILEDCHECK(Save_HierarchyNodes(szFullPath));

			if(0 != iAnimState || TYPE_ANIM_GROUP != m_eModelType)
				FAILEDCHECK(Save_Animation(szModelFilePath, iAnimGroup));
		}
	}
	else
	{
		string strFilePath(szModelFilePath);
		if (strFilePath.substr(strFilePath.length() - 7).compare("Effect/")) // Effect 관련파일이 아닌것만 Tex저장.
		{
			FAILEDCHECK(Load_Materials(szFullPath));

			// Read
			if (TYPE_NONANIM != m_eModelType
				&& TYPE_NONANIM_INSTANCE != m_eModelType)
			{
				FAILEDCHECK(Load_HierarchyNodes(szFullPath));

				sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
					{
						return pSour->Get_Depth() < pDest->Get_Depth();
					});

				sort(m_HierarchyNodesData.begin(), m_HierarchyNodesData.end(), [](HIERARCHYNODEDATADESC tSour, HIERARCHYNODEDATADESC tDest)
					{
						return tSour.iDepth < tDest.iDepth;
					});
			}
		}

		FAILEDCHECK(Load_MeshContainers(szFullPath));

		if (TYPE_ANIM == m_eModelType
			|| TYPE_ANIM_INSTANCE == m_eModelType
			|| TYPE_ANIM_GROUP == m_eModelType)
		{
			if (0 != iAnimState || TYPE_ANIM_GROUP != m_eModelType)
				FAILEDCHECK(Load_Animation(szModelFilePath, iAnimGroup));
		}
	}

	return S_OK;
}



HRESULT CModel::Initialize(void* pArg, CModel* pPrototype)
{
	if (TYPE_NONANIM != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		_uint iHierarchyNodeIndex = 0;

		for (_uint i = 0; i < pPrototype->m_iNumHierarchyNode; ++i)
		{
			CHierarchyNode* pHierarchyNode = pPrototype->Get_HierarchyNode(i)->Clone();

			if (nullptr == pHierarchyNode)
				continue;

			HIERARCHYNODEDATADESC tDesc;

			tDesc.iIndex = pHierarchyNode->Get_CurrentIndex();
			tDesc.iDepth = pHierarchyNode->Get_Depth();
			tDesc.iParentIndex = pHierarchyNode->Get_ParentsIndex();
			strcpy_s(tDesc.szName, pHierarchyNode->Get_Name());
			XMStoreFloat4x4(&tDesc.TransformationMatrix, pHierarchyNode->Get_TransformationMatrix());
			XMStoreFloat4x4(&tDesc.OffsetMatrix, pHierarchyNode->Get_OffsetMatrix());
			XMStoreFloat4x4(&tDesc.CombinedTransformationMatrix, pHierarchyNode->Get_CombinedMatrix());

			m_HierarchyNodesData.push_back(tDesc);
			m_HierarchyNodes.push_back(pHierarchyNode);
		}

		for (auto iter : m_HierarchyNodes)
		{
			iter->SetParent(this);
		}
	}

	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		for (auto& pPrototypeMeshContainer : pPrototype->m_MeshContainers)
		{
			CMeshContainer* pMeshContainer = (CMeshContainer*)pPrototypeMeshContainer->Clone();
			NULLCHECK(pMeshContainer);

			m_MeshContainers.push_back(pMeshContainer);

			if (TYPE_NONANIM != m_eModelType
				&& TYPE_NONANIM_INSTANCE != m_eModelType)
				pMeshContainer->SetUp_BonesPtr(this);
		}
	}
	else
	{
		for (auto& pPrototypeMeshContainer : pPrototype->m_MeshInstanceContainers)
		{
			CMesh_Instance* pMeshContainer = (CMesh_Instance*)pPrototypeMeshContainer->Clone(pArg);
			NULLCHECK(pMeshContainer);

			m_MeshInstanceContainers.push_back(pMeshContainer);

			if (TYPE_NONANIM != m_eModelType
				&& TYPE_NONANIM_INSTANCE != m_eModelType)
				pMeshContainer->SetUp_BonesPtr(this);
		}
	}

	if (TYPE_ANIM == m_eModelType
		|| TYPE_ANIM_INSTANCE == m_eModelType)
	{
		for (auto& pPrototypeAnimation : pPrototype->m_Animations)
		{
			CAnimation* pAnimation = pPrototypeAnimation->Clone(this);
			NULLCHECK(pAnimation);
			m_Animations.push_back(pAnimation);
		}
	}
	else if (TYPE_ANIM_GROUP == m_eModelType)
	{
		m_pAnimManager = CAnimation_Manager::Get_Instance();
	}

	//m_pAnimOffsetMat = new _float4x4[m_iNumAnimations];
	//for (size_t i = 0; i < m_iNumAnimations; ++i)
	//{
	//	XMStoreFloat4x4(&m_pAnimOffsetMat[i], XMMatrixIdentity());
	//}

	XMStoreFloat4x4(&m_RotateOffset, XMMatrixIdentity());

	return S_OK;
}

HRESULT CModel::Play_Animation(_float fTimeDelta, string strRootBoneName, _float4x4* pOut)
{
	m_iNumAnimations = _uint(m_Animations.size());

	if (strcmp(strRootBoneName.c_str(), ""))
		m_strBoneName = strRootBoneName;

	if (m_iCurrentAnimationIndex >= m_iNumAnimations)
		return E_FAIL;

	if (m_iCurrentAnimationIndex != m_iPreAnimationIndex || m_bAnimGroupChange)
	{
		m_fTimeAcc += fTimeDelta;

		m_Animations[m_iCurrentAnimationIndex]->Set_ChangeAnim();

		if (m_fTimeRatio > m_fTimeAcc)
		{
			map<const char*, KEYFRAME> PreKeyframe;
			vector<CChannel*> CurKeyframe;
			PreKeyframe = m_Animations[m_iPreAnimationIndex]->GetKeyframes();
			CurKeyframe = m_Animations[m_iCurrentAnimationIndex]->GetChannel();
			//if(!m_bAnimGroupChange)
			//{
			//	PreKeyframe = m_Animations[m_iPreAnimationIndex]->GetKeyframes();
			//	CurKeyframe = m_Animations[m_iCurrentAnimationIndex]->GetChannel();
			//}
			//else
			//{
			//	PreKeyframe = m_PreAnimations[m_iPreAnimationIndex]->GetKeyframes();
			//	CurKeyframe = m_Animations[m_iCurrentAnimationIndex]->GetChannel();
			//}

			for (size_t i = 0; i < CurKeyframe.size(); ++i)
			{
				map<const char*, KEYFRAME>::iterator iter = PreKeyframe.begin();// PreKeyframe.end(), strcmp(, iter->first) );

				for (; iter != PreKeyframe.end(); )
				{
					if (!strcmp(CurKeyframe[i]->Get_ChannelName(), iter->first))
					{
						break;
					}
					else
						++iter;
				}

				_vector			vScale, vRotation, vPosition;
				_vector			vSourScale, vSourRotation, vSourPosition;
				_vector			vDestScale, vDestRotation, vDestPosition;

				if (PreKeyframe.end() == iter)
					continue;

				KEYFRAME	CurKeyFramePtr = CurKeyframe[i]->Get_CurrentKeyFrame();

				if (!strcmp(m_strBoneName.c_str(), CurKeyframe[i]->Get_ChannelName())
					&& m_bRootEasing)
				{

					vScale = XMLoadFloat3(&CurKeyFramePtr.vScale);
					vRotation = XMLoadFloat4(&CurKeyFramePtr.vRotation);
					vPosition = XMLoadFloat3(&CurKeyFramePtr.vPosition);

					_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

					m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(CurKeyframe[i], TransformationMatrix);

					continue;
				}

				vSourScale = XMLoadFloat3(&iter->second.vScale);
				vSourRotation = XMLoadFloat4(&iter->second.vRotation);
				vSourPosition = XMLoadFloat3(&iter->second.vPosition);


				vDestScale = XMLoadFloat3(&CurKeyFramePtr.vScale);
				vDestRotation = XMLoadFloat4(&CurKeyFramePtr.vRotation);
				vDestPosition = XMLoadFloat3(&CurKeyFramePtr.vPosition);


				_float fRatio = m_fTimeAcc / m_fTimeRatio;


				vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
				vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
				vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
				vPosition = XMVectorSetW(vPosition, 1.f);

				_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

				m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(CurKeyframe[i], TransformationMatrix);
			}
		}
		else
		{
			m_Animations[m_iPreAnimationIndex]->Reset_KeyFrame();
			//if (!m_bAnimGroupChange)
			//{
			//	m_Animations[m_iPreAnimationIndex]->Reset_KeyFrame();
			//}
			//else
			//{
			//	m_PreAnimations[m_iPreAnimationIndex]->Reset_KeyFrame();
			//}

			m_bAnimGroupChange = false;
			m_iPreAnimationIndex = m_iCurrentAnimationIndex;
			m_fTimeAcc = 0.f;
		}
	}
	else
	{
		if (m_Animations[m_iCurrentAnimationIndex]->Get_IsFinished())
		{
			m_Animations[m_iCurrentAnimationIndex]->Reset_KeyFrame();
		}

		m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(fTimeDelta);
	}

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix(strRootBoneName, pOut);
	}


	//if (strcmp(strRootBoneName.c_str(), ""))
	//{
	//	CHierarchyNode* pTargetNode = Find_HierarcyNode(strRootBoneName.c_str());
	//	_matrix		CombinedMat = pTargetNode->Get_RealCombinedMatrix()* pTargetNode->Get_TransformMatrix();
	//	XMStoreFloat3(&m_vCurTargetPos, CombinedMat.r[3]);
	//	XMStoreFloat3(&m_vAnimPosDiff, XMLoadFloat3(&m_vCurTargetPos) - XMLoadFloat3(&m_vPreTargetPos));

	//	COUT_FLOAT3(m_vAnimPosDiff);

	//	m_vPreTargetPos = m_vCurTargetPos;
	//}



	return S_OK;
}

HRESULT CModel::Play_Animation_Easing(_float fTimeDelta, string strRootBoneName, _float4x4* pOut, _uint iEasingtype, CTransform* pTransform)
{
	m_iNumAnimations = _uint(m_Animations.size());
	m_iEasingtype = iEasingtype;

	if (strcmp(strRootBoneName.c_str(), ""))
		m_strBoneName = strRootBoneName;

	if (m_iCurrentAnimationIndex >= m_iNumAnimations)
		return E_FAIL;

	if (m_iCurrentAnimationIndex != m_iPreAnimationIndex || m_bAnimGroupChange)
	{
		m_fTimeAcc += fTimeDelta;

		m_Animations[m_iCurrentAnimationIndex]->Set_ChangeAnim();

		if (m_fTimeRatio > m_fTimeAcc)
		{
			map<const char*, KEYFRAME> PreKeyframe;
			vector<CChannel*> CurKeyframe;
			PreKeyframe = m_Animations[m_iPreAnimationIndex]->GetKeyframes();
			CurKeyframe = m_Animations[m_iCurrentAnimationIndex]->GetChannel();

			for (size_t i = 0; i < CurKeyframe.size(); ++i)
			{
				map<const char*, KEYFRAME>::iterator iter = PreKeyframe.begin();// PreKeyframe.end(), strcmp(, iter->first) );

				for (; iter != PreKeyframe.end(); )
				{
					if (!strcmp(CurKeyframe[i]->Get_ChannelName(), iter->first))
					{
						break;
					}
					else
						++iter;
				}

				_vector			vScale, vRotation, vPosition;
				_vector			vSourScale, vSourRotation, vSourPosition;
				_vector			vDestScale, vDestRotation, vDestPosition;

				if (PreKeyframe.end() == iter)
					continue;

				KEYFRAME	CurKeyFramePtr = CurKeyframe[i]->Get_CurrentKeyFrame();

				if (!strcmp(m_strBoneName.c_str(), CurKeyframe[i]->Get_ChannelName())
					&& m_bRootEasing)
				{

					vScale = XMLoadFloat3(&CurKeyFramePtr.vScale);
					vRotation = XMLoadFloat4(&CurKeyFramePtr.vRotation);
					vPosition = XMLoadFloat3(&CurKeyFramePtr.vPosition);

					_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

					m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(CurKeyframe[i], TransformationMatrix);

					continue;
				}



				vSourScale = XMLoadFloat3(&iter->second.vScale);
				vSourRotation = XMLoadFloat4(&iter->second.vRotation);
				vSourPosition = XMLoadFloat3(&iter->second.vPosition);


				if (!strcmp(strRootBoneName.c_str(), CurKeyframe[i]->Get_ChannelName()))
				{
					m_vOriDestRotation = CurKeyFramePtr.vRotation;
					m_pTargetChannel = CurKeyframe[i];
					m_TargetKeyframe = CurKeyFramePtr;
					vDestRotation = XMQuaternionRotationMatrix(XMMatrixRotationQuaternion(XMLoadFloat4(&CurKeyFramePtr.vRotation)) * XMLoadFloat4x4(&m_RotateOffset));
				}
				else
				{
					vDestRotation = XMLoadFloat4(&CurKeyFramePtr.vRotation);
				}

				vDestScale = XMLoadFloat3(&CurKeyFramePtr.vScale);
				vDestPosition = XMLoadFloat3(&CurKeyFramePtr.vPosition);


				//_float fRatio = m_fTimeAcc / m_fTimeRatio;

				_float fRatio = XMVectorGetX(CEasing_Utillity::Easing(m_iEasingtype, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), min(m_fTimeAcc, m_fTimeRatio), m_fTimeRatio));

				vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
				vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
				vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
				vPosition = XMVectorSetW(vPosition, 1.f);

				_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

				m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(CurKeyframe[i], TransformationMatrix);
			}
		}
		else
		{
			m_Animations[m_iPreAnimationIndex]->Reset_KeyFrame();

			//if (pTransform)
			//	pTransform->Turn(XMLoadFloat4x4(&m_RotateOffset));

			//XMStoreFloat4x4(&m_RotateOffset, XMMatrixIdentity());
			m_bChangeAnimation = true;
			m_bAnimGroupChange = false;
			m_iPreAnimationIndex = m_iCurrentAnimationIndex;
			m_fTimeAcc = 0.f;
		}
	}
	else
	{

		m_bTransform_Turn_End = false;

		if (m_bChangeAnimation)
		{
			if (pTransform)
				pTransform->Turn(XMLoadFloat4x4(&m_RotateOffset));

			//_vector vRotation = XMLoadFloat4(&m_TargetKeyframe.vRotation);
			//_vector vScale = XMLoadFloat3(&m_TargetKeyframe.vScale);
			//_vector vPosition = XMLoadFloat3(&m_TargetKeyframe.vPosition);

			//_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

			//m_Animations[m_iPreAnimationIndex]->Update_TransformationMatrices(m_pTargetChannel, TransformationMatrix);

			m_bChangeAnimation = false;

			m_bTransform_Turn_End = true;
		}


		if (m_Animations[m_iCurrentAnimationIndex]->Get_IsFinished())
		{
			m_bChangeAnimation = true;
			m_Animations[m_iCurrentAnimationIndex]->Reset_KeyFrame();
		}
		//XMStoreFloat4x4(&m_RotateOffset, XMMatrixIdentity());


		m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(fTimeDelta);
	}

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix(strRootBoneName, pOut);
	}

	return S_OK;
}

HRESULT CModel::Set_AnimMotion(string strRootBoneName, _float4x4* pOut)
{
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix(strRootBoneName, pOut);

	return S_OK;
}

HRESULT CModel::Set_AnimationGroup(_uint iAnimGroup, _uint iAnimState)
{
	if (nullptr == m_pAnimManager)
		return E_FAIL;

	if (TYPE_ANIM_GROUP == m_eModelType)
	{
		if (0 != m_Animations.size() && m_iAnimState != iAnimState)
		{
			m_bAnimGroupChange = true;
			m_iPreAnimGroup = m_iAnimGroup;
			m_iPreAnimState = m_iAnimState;
			//m_PreAnimations = m_pAnimManager->Get_Animations(m_iPreAnimGroup, m_iPreAnimState);;
		}
		//m_Animations = m_pAnimManager->Get_Animations(iAnimGroup, iAnimState);
		m_iAnimGroup = iAnimGroup;
		m_iAnimState = iAnimState;
	}

	return S_OK;
}

HRESULT CModel::Set_AnimationGroup(_uint iAnimGroup)
{
	m_Animations = m_pAnimManager->Get_Animations(iAnimGroup);

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex, const char* pConstantBoneName, _bool bUseFrustum)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	/* 그리고자하는 메시컨테이너에 영향을 주는 뼈들의 행렬을 담아준다. */
	_float4x4			BoneMatrices[256];


	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
		m_MeshContainers[iMeshContainerIndex]->SetUp_BoneMatices(BoneMatrices, XMLoadFloat4x4(&m_TransformMatrix));
	else
		m_MeshInstanceContainers[iMeshContainerIndex]->SetUp_BoneMatices(BoneMatrices, XMLoadFloat4x4(&m_TransformMatrix)/* * XMLoadFloat4x4(&m_pAnimOffsetMat[m_iCurrentAnimationIndex])*/);

	if (0 != m_iNumAnimations)
		pShader->Set_RawValue(pConstantBoneName, BoneMatrices, sizeof(_float4x4) * 256);

	pShader->Begin(iPassIndex);

	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
		m_MeshContainers[iMeshContainerIndex]->Render();
	else
	{
		m_MeshInstanceContainers[iMeshContainerIndex]->Update(bUseFrustum);
		m_MeshInstanceContainers[iMeshContainerIndex]->Render();
	}

	return S_OK;
}

HRESULT CModel::Bind_SRV(CShader* pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex;
	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_MaterialIndex();
	}
	else
	{
		iMaterialIndex = m_MeshInstanceContainers[iMeshContainerIndex]->Get_MaterialIndex();
	}

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (nullptr != m_Materials[iMaterialIndex].pTextures[eType])
		return m_Materials[iMaterialIndex].pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);
	else
		return S_OK;
}

HRESULT CModel::Check_File(const char* pModelFilePath, const char* pModelFileName)
{
	char	szFullPath[MAX_PATH] = "";

	// 확장자 부분 삭제
	strcat_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint lenFullPath = (_uint)strlen(szFullPath);

	szFullPath[lenFullPath - 3] = '\0';

	// 파일이름 생성
	char	szSkeleton[MAX_PATH] = "";
	char	szMesh[MAX_PATH] = "";
	char	szMaterial[MAX_PATH] = "";

	strcpy_s(szSkeleton, szFullPath);
	strcat_s(szSkeleton, "bHier");
	strcpy_s(szMesh, szFullPath);
	strcat_s(szMesh, "bMesh");
	strcpy_s(szMaterial, szFullPath);
	strcat_s(szMaterial, "bMat");

	HANDLE	hFile;
	if (TYPE_ANIM == m_eModelType)
	{
		_tchar t0[MAX_PATH];
		swprintf(t0, MAX_PATH, TEXT("%hs"), szSkeleton);
		hFile = CreateFile(t0, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}
		CloseHandle(hFile);
	}

	_tchar t1[MAX_PATH];
	swprintf(t1, MAX_PATH, TEXT("%hs"), szMesh);
	hFile = CreateFile(t1, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
		return E_FAIL;
	}
	CloseHandle(hFile);

	string strFilePath(pModelFilePath);
	if (strFilePath.substr(strFilePath.length() - 7).compare("Effect/")) // Effect 관련파일이 아닌것만 Tex확인.
	{
		_tchar t2[MAX_PATH];
		swprintf(t2, MAX_PATH, TEXT("%hs"), szMaterial);
		hFile = CreateFile(t2, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}
		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_eModelType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_TransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char			szFullPath[MAX_PATH] = "";

			aiString		strPath;

			if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, _int(strlen(szFullPath)), szTextureFilePath, MAX_PATH);

			Material.pTextures[j] = CTexture::Create(szTextureFilePath);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;
		}

		m_Materials.push_back(Material);
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode* pAINode, CHierarchyNode* pParent, _int iParentIndex, _uint* iIndex, _uint iDepth)
{
	HIERARCHYNODEDATADESC tDesc;
	ZeroMemory(&tDesc, sizeof(HIERARCHYNODEDATADESC));

	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pAINode, pParent, iDepth, m_TransformMatrix, iIndex);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	tDesc.iDepth = pHierarchyNode->Get_Depth();
	tDesc.iIndex = pHierarchyNode->Get_CurrentIndex();
	tDesc.iParentIndex = pHierarchyNode->Get_ParentsIndex();
	strcpy_s(tDesc.szName, pHierarchyNode->Get_Name());
	XMStoreFloat4x4(&tDesc.OffsetMatrix, pHierarchyNode->Get_OffsetMatrix());
	XMStoreFloat4x4(&tDesc.CombinedTransformationMatrix, pHierarchyNode->Get_CombinedMatrix());
	XMStoreFloat4x4(&tDesc.TransformationMatrix, pHierarchyNode->Get_TransformationMatrix());

	//strcpy_s(tDesc.szName, pAINode->mName.data);
	//
	//memcpy(&tDesc.TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	//XMStoreFloat4x4(&tDesc.TransformationMatrix,
	//	XMMatrixTranspose(XMLoadFloat4x4(&tDesc.TransformationMatrix)));
	//
	//XMStoreFloat4x4(&tDesc.OffsetMatrix, XMMatrixIdentity());
	//
	//XMStoreFloat4x4(&tDesc.CombinedTransformationMatrix, XMMatrixIdentity());

	m_HierarchyNodes.push_back(pHierarchyNode);

	m_HierarchyNodesData.push_back(tDesc);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		++(*iIndex);
		Create_HierarchyNodes(pAINode->mChildren[i], pHierarchyNode, tDesc.iIndex, iIndex, iDepth + 1);
	}

	return S_OK;
}

HRESULT CModel::Create_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Save_HierarchyNodes(const char* pModelFilePath)
{
	char	szFullPath[MAX_PATH] = "";

	// 확장자 부분 삭제
	strcat_s(szFullPath, pModelFilePath);

	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	// 파일이름 생성
	char	szMesh[MAX_PATH] = "";

	strcpy_s(szMesh, szFullPath);
	strcat_s(szMesh, "bHier");

	_tchar t1[MAX_PATH];
	swprintf(t1, MAX_PATH, TEXT("%hs"), szMesh);

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(t1, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	m_iNumHierarchyNode = _uint(m_HierarchyNodes.size());
	WriteFile(hFile, &m_iNumHierarchyNode, sizeof(_uint), &dwByte, nullptr);

	for (auto& iter : m_HierarchyNodes)
	{
		HIERARCHYNODEDATADESC tDesc;
		ZeroMemory(&tDesc, sizeof(HIERARCHYNODEDATADESC));

		tDesc.iDepth = iter->Get_Depth();
		tDesc.iIndex = iter->Get_CurrentIndex();
		tDesc.iParentIndex = iter->Get_ParentsIndex();
		strcpy_s(tDesc.szName, iter->Get_Name());
		XMStoreFloat4x4(&tDesc.OffsetMatrix, iter->Get_OffsetMatrix());
		XMStoreFloat4x4(&tDesc.CombinedTransformationMatrix, iter->Get_CombinedMatrix());
		XMStoreFloat4x4(&tDesc.TransformationMatrix, iter->Get_TransformationMatrix());

		_int iParentIndex;
		for (_uint i = 0; i < m_HierarchyNodes.size(); ++i)
		{
			if (-1 == tDesc.iParentIndex)
			{
				iParentIndex = -1;
				break;
			}

			if (m_HierarchyNodesData[i].iIndex == tDesc.iParentIndex)
			{
				iParentIndex = i;
				break;
			}
		}

		WriteFile(hFile, &tDesc.iDepth, sizeof(_uint), &dwByte, nullptr);

		DWORD iLen = (DWORD)strlen(tDesc.szName) + 1;
		WriteFile(hFile, &iLen, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, &tDesc.szName, sizeof(char) * iLen, &dwByte, nullptr);

		WriteFile(hFile, &tDesc.TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);
		WriteFile(hFile, &tDesc.OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
		WriteFile(hFile, &tDesc.CombinedTransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);

		WriteFile(hFile, &tDesc.iIndex, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &iParentIndex, sizeof(_int), &dwByte, nullptr);

		iter->SetParentIndex(iParentIndex);

		//m_HierarchyNodesData.push_back(tDesc);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Save_MeshContainers(const char* pFullFilePath)
{
	char	szFullPath[MAX_PATH] = "";

	// 확장자 부분 삭제
	strcat_s(szFullPath, pFullFilePath);

	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	// 파일이름 생성
	char	szMesh[MAX_PATH] = "";

	strcpy_s(szMesh, szFullPath);
	strcat_s(szMesh, "bMesh");

	_tchar t1[MAX_PATH];
	swprintf(t1, MAX_PATH, TEXT("%hs"), szMesh);

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(t1, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	WriteFile(hFile, &m_iNumMeshContainers, sizeof(_uint), &dwByte, nullptr);

	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			const aiMesh* pAIMesh = m_pAIScene->mMeshes[i];

			CMeshContainer* pMeshContainer = CMeshContainer::Create(m_eModelType, pAIMesh, this, XMLoadFloat4x4(&m_TransformMatrix), &dwByte, hFile);
			if (nullptr == pMeshContainer)
			{
				CloseHandle(hFile);
				return E_FAIL;
			}

			m_MeshContainers.push_back(pMeshContainer);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			const aiMesh* pAIMesh = m_pAIScene->mMeshes[i];

			CMesh_Instance* pMeshContainer = CMesh_Instance::Create(m_eModelType, pAIMesh, this, XMLoadFloat4x4(&m_TransformMatrix), &dwByte, hFile);
			if (nullptr == pMeshContainer)
			{
				CloseHandle(hFile);
				return E_FAIL;
			}

			m_MeshInstanceContainers.push_back(pMeshContainer);
		}
	}


	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Save_Materials(const char* pModelFilePath, const char* pModelFileName)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	char	szFullPath[MAX_PATH] = "";

	strcat_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	// 확장자 부분 삭제
	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	strcat_s(szFullPath, "bMat");

	_tchar t1[MAX_PATH];
	swprintf(t1, MAX_PATH, TEXT("%hs"), szFullPath);

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(t1, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	WriteFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char			szFullPath[MAX_PATH] = "";

			aiString		strPath;

			if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
			{
				DWORD iTemp = 0;
				WriteFile(hFile, &iTemp, sizeof(DWORD), &dwByte, nullptr);
				continue;
			}

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			DWORD iLen = (DWORD)strlen(szFullPath) + 1;
			WriteFile(hFile, &iLen, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, &szFullPath, sizeof(char) * iLen, &dwByte, nullptr);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, int(strlen(szFullPath)), szTextureFilePath, MAX_PATH);

			Material.pTextures[j] = CTexture::Create(szTextureFilePath);
			if (nullptr == Material.pTextures[j])
			{
				CloseHandle(hFile);
				return E_FAIL;
			}
		}

		m_Materials.push_back(Material);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Save_Animation(const char* pModelFilePath, _uint iAnimGroup)
{
	char	szFullPath[MAX_PATH] = "";

	strcat_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, "Animation.bin");

	_tchar t2[MAX_PATH];
	swprintf(t2, MAX_PATH, TEXT("%hs"), szFullPath);

	DWORD			dwByte = 0;
	HANDLE			hFile = CreateFile(t2, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		return E_FAIL;
	}

	m_iNumAnimations = m_pAIScene->mNumAnimations - 1;
	WriteFile(hFile, &m_iNumAnimations, sizeof(_uint), &dwByte, nullptr);

	if (TYPE_ANIM_GROUP == m_eModelType)
	{
		m_iAnimGroup = iAnimGroup;
		m_pAnimManager = CAnimation_Manager::Get_Instance();
	}


	for (_uint i = 1; i < m_pAIScene->mNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this, &dwByte, hFile);

		if (nullptr == pAnimation)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		if (TYPE_ANIM_GROUP == m_eModelType)
			m_pAnimManager->Add_Animation(iAnimGroup, pAnimation);
		else
			m_Animations.push_back(pAnimation);
	}

	//if (TYPE_ANIM_GROUP == m_eModelType)
	//{
	//	m_pAnimManager = CAnimation_Manager::Get_Instance();
	//	m_pAnimManager->Add_AnimationGroup(iAnimGroup, iAnimState, m_Animations);
	//}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Load_HierarchyNodes(const char* pFullFilePath)
{
	//m_pNodePath;
	char	szFullPath[MAX_PATH] = "";
	char	szFullPathBone[MAX_PATH] = "";

	strcat_s(szFullPath, pFullFilePath);

	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	strcat_s(szFullPath, "bHier");
	strcpy_s(szFullPathBone, szFullPath);

	_tchar		szFilePath[MAX_PATH] = TEXT("");

	MultiByteToWideChar(CP_ACP, 0, szFullPathBone, (int)strlen(szFullPathBone), szFilePath, MAX_PATH);

	// 파일 개방 함수
	HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("CModel - LoadData() : hFile failed");
		return E_FAIL;
	}

	DWORD			dwByte = 0;

	//하이라키노드
	ReadFile(hFile, &m_iNumHierarchyNode, sizeof(_uint), &dwByte, nullptr);
	for (_uint i = 0; i < m_iNumHierarchyNode; ++i)
	{
		CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(hFile, &dwByte, m_TransformMatrix);
		NULLCHECK(pHierarchyNode);

		HIERARCHYNODEDATADESC tDesc;
		tDesc.iDepth = pHierarchyNode->Get_Depth();
		tDesc.iIndex = pHierarchyNode->Get_CurrentIndex();
		tDesc.iParentIndex = pHierarchyNode->Get_ParentsIndex();
		strcpy_s(tDesc.szName, pHierarchyNode->Get_Name());
		XMStoreFloat4x4(&tDesc.OffsetMatrix, pHierarchyNode->Get_OffsetMatrix());
		XMStoreFloat4x4(&tDesc.CombinedTransformationMatrix, pHierarchyNode->Get_CombinedMatrix());
		XMStoreFloat4x4(&tDesc.TransformationMatrix, pHierarchyNode->Get_TransformationMatrix());

		m_HierarchyNodes.push_back(pHierarchyNode);
		m_HierarchyNodesData.push_back(tDesc);
	}

	for (auto iter : m_HierarchyNodes)
	{
		iter->SetParent(this);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Load_MeshContainers(const char* pFullFilePath)
{
	//m_pNodePath;
	char	szFullPath[MAX_PATH] = "";
	char	szFullPathBone[MAX_PATH] = "";

	strcat_s(szFullPath, pFullFilePath);

	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	strcat_s(szFullPath, "bMesh");
	strcpy_s(szFullPathBone, szFullPath);

	_tchar		szFilePath[MAX_PATH] = TEXT("");

	MultiByteToWideChar(CP_ACP, 0, szFullPathBone, (int)strlen(szFullPathBone), szFilePath, MAX_PATH);

	// 파일 개방 함수
	HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("CModel - LoadData() : hFile failed");
		return E_FAIL;
	}

	DWORD			dwByte = 0;

	//매시컨테이너
	ReadFile(hFile, &m_iNumMeshContainers, sizeof(_uint), &dwByte, nullptr);

	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			CMeshContainer* pMeshContainer = CMeshContainer::Create(hFile, &dwByte, this, XMLoadFloat4x4(&m_TransformMatrix));
			if (nullptr == pMeshContainer)
				return E_FAIL;

			m_MeshContainers.push_back(pMeshContainer);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			CMesh_Instance* pMeshContainer = CMesh_Instance::Create(hFile, &dwByte, this, XMLoadFloat4x4(&m_TransformMatrix));
			if (nullptr == pMeshContainer)
				return E_FAIL;

			m_MeshInstanceContainers.push_back(pMeshContainer);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Load_Materials(const char* pFullFilePath)
{
	//m_pNodePath;
	char	szFullPath[MAX_PATH] = "";
	char	szFullPathBone[MAX_PATH] = "";

	strcat_s(szFullPath, pFullFilePath);

	_uint lenFullPath = (_uint)strlen(szFullPath);
	szFullPath[lenFullPath - 3] = '\0';

	strcat_s(szFullPath, "bMat");
	strcpy_s(szFullPathBone, szFullPath);

	_tchar		szFilePath[MAX_PATH] = TEXT("");

	MultiByteToWideChar(CP_ACP, 0, szFullPathBone, (int)strlen(szFullPathBone), szFilePath, MAX_PATH);

	// 파일 개방 함수
	HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("CModel - LoadData() : hFile failed");
		return E_FAIL;
	}

	DWORD			dwByte = 0;

	//매터리얼
	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			DWORD iLen;
			ReadFile(hFile, &iLen, sizeof(DWORD), &dwByte, nullptr);

			if (0 == iLen)
				continue;

			char			szFullPath[MAX_PATH] = "";

			ReadFile(hFile, &szFullPath, sizeof(char) * iLen, &dwByte, nullptr);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, int(strlen(szFullPath)), szTextureFilePath, MAX_PATH);

			Material.pTextures[j] = CTexture::Create(szTextureFilePath);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;
		}
		m_Materials.push_back(Material);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Load_Animation(const char* pModelFilePath, _uint iAnimGroup)
{
	//m_pNodePath;
	char	szFullPath[MAX_PATH] = "";

	strcat_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, "Animation.bin");

	_tchar		szFilePath[MAX_PATH] = TEXT("");

	MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szFilePath, MAX_PATH);

	// 파일 개방 함수
	HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("CModel - LoadData() : hFile failed");
		return E_FAIL;
	}

	DWORD			dwByte = 0;

	//애니메이션

	ReadFile(hFile, &m_iNumAnimations, sizeof(_uint), &dwByte, nullptr);

	if (TYPE_ANIM_GROUP == m_eModelType)
	{
		m_iAnimGroup = iAnimGroup;
		m_pAnimManager = CAnimation_Manager::Get_Instance();
	}

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(hFile, &dwByte, this);

		if (nullptr == pAnimation)
			return E_FAIL;

		if (TYPE_ANIM_GROUP == m_eModelType)
			m_pAnimManager->Add_Animation(iAnimGroup, pAnimation);
		else
			m_Animations.push_back(pAnimation);
	}

	//if (TYPE_ANIM_GROUP == m_eModelType)
	//{
	//	m_pAnimManager = CAnimation_Manager::Get_Instance();
	//	m_pAnimManager->Add_AnimationGroup(iAnimGroup, iAnimState, m_Animations);
	//}

	CloseHandle(hFile);

	return S_OK;
}

CModel* CModel::Create(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix, _uint iAnimGroup, _uint iAnimState)
{
	CModel* pInstance = new CModel();

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName, TransformMatrix, iAnimGroup, iAnimState)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg, this)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	if (TYPE_ANIM_GROUP != m_eModelType)
	{
		for (auto& pAnimation : m_Animations)
			Safe_Release(pAnimation);
	}
	//m_Animations.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	//m_HierarchyNodes.clear();

	//m_HierarchyNodesData.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_Release(pTexture);
	}
	//m_Materials.clear();

	if (TYPE_ANIM_INSTANCE != m_eModelType
		&& TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		for (auto& pMeshContainer : m_MeshContainers)
			Safe_Release(pMeshContainer);

		//m_MeshContainers.clear();
	}
	else
	{
		for (auto& pMeshContainer : m_MeshInstanceContainers)
			Safe_Release(pMeshContainer);

		//m_MeshInstanceContainers.clear();
	}

	//Safe_Delete_Array(m_pAnimOffsetMat);
	
	if (false == m_isCloned)
		m_Importer.FreeScene();

}
