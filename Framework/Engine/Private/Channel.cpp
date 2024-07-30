#include "..\Public\Channel.h"
#include "HIerarchyNode.h"
#include "Model.h"

CChannel::CChannel()
{
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrentKeyFrame = 0;
}

CChannel::CChannel(const CChannel& rhs)
	: m_iNumKeyframes(rhs.m_iNumKeyframes)
	, m_KeyFrames(rhs.m_KeyFrames)
	, m_iCurrentKeyFrame(rhs.m_iCurrentKeyFrame)
	, m_KeyFrame(rhs.m_KeyFrame)
{
	strcpy_s(m_szName, rhs.m_szName);
}

vector<KEYFRAME> CChannel::Get_KeyFrames()
{
	return m_KeyFrames;
}

KEYFRAME CChannel::Get_KeyFrame()
{
	return m_KeyFrame;
}

void CChannel::Set_HierarchyNodeMatrix(_fmatrix TransformationMatrix)
{
	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransformationMatrix);
}
HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel* pModel, DWORD* dwByte, HANDLE hFile)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);
	DWORD iLen = (DWORD)strlen(m_szName) + 1;
	WriteFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	WriteFile(hFile, m_szName, sizeof(char) * iLen, dwByte, nullptr);

	m_pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	//WriteFile(hFile, &iHierarchyIndex, sizeof(_uint), dwByte, nullptr);
	WriteFile(hFile, &m_iNumKeyframes, sizeof(_uint), dwByte, nullptr);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = (_float)pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		WriteFile(hFile, &KeyFrame, sizeof(KEYFRAME), dwByte, nullptr);

		m_KeyFrames.push_back(KeyFrame);

		if (0 == i)
		{
			memcpy(&m_KeyFrame, &KeyFrame, sizeof(KEYFRAME));
		}
	}

	return S_OK;
}

HRESULT CChannel::Initialize(HANDLE hFile, DWORD* dwByte, CModel* pModel)
{
	DWORD iLen;
	ReadFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	ReadFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

	//_uint iHierarchyNodeIndex;
	//ReadFile(hFile, &iHierarchyNodeIndex, sizeof(_uint), dwByte, nullptr);
	if(nullptr != pModel)
	{
		m_pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
		if (nullptr == m_pHierarchyNode)
			return E_FAIL;
	}

	//Safe_AddRef(m_pHierarchyNode);

	ReadFile(hFile, &m_iNumKeyframes, sizeof(_uint), dwByte, nullptr);

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		KeyFrame;
		ReadFile(hFile, &KeyFrame, sizeof(KEYFRAME), dwByte, nullptr);

		m_KeyFrames.push_back(KeyFrame);

		if (0 == i)
		{
			memcpy(&m_KeyFrame, &KeyFrame, sizeof(KEYFRAME));
		}
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;

	if (fCurrentTime > m_KeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fCurrentTime > m_KeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrame].fTime)
			/ (m_KeyFrames[m_iCurrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	XMStoreFloat3(&m_KeyFrame.vScale, vScale);
	XMStoreFloat4(&m_KeyFrame.vRotation, vRotation);
	XMStoreFloat3(&m_KeyFrame.vPosition, vPosition);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Set_TimeCurrentKeyFrame(_float fTime)
{
	_vector			vScale, vRotation, vPosition;
	_uint			iKeyFrame = 0;

	while (fTime > m_KeyFrames[iKeyFrame + 1].fTime)
	{
		++iKeyFrame;
	}

	m_iCurrentKeyFrame = iKeyFrame;

	vScale = XMLoadFloat3(&m_KeyFrames[iKeyFrame].vScale);
	vRotation = XMLoadFloat4(&m_KeyFrames[iKeyFrame].vRotation);
	vPosition = XMLoadFloat3(&m_KeyFrames[iKeyFrame].vPosition);
	vPosition = XMVectorSetW(vPosition, 1.f);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransformationMatrix);
}


void CChannel::Set_NumCurrentKeyFrame(_uint iKeyFrame, _float* fTime)
{
	_vector			vScale, vRotation, vPosition;

	m_iCurrentKeyFrame = iKeyFrame;

	vScale = XMLoadFloat3(&m_KeyFrames[iKeyFrame].vScale);
	vRotation = XMLoadFloat4(&m_KeyFrames[iKeyFrame].vRotation);
	vPosition = XMLoadFloat3(&m_KeyFrames[iKeyFrame].vPosition);
	vPosition = XMVectorSetW(vPosition, 1.f);
	*fTime = m_KeyFrames[iKeyFrame].fTime;

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Set_HierarchyNode(CModel* pModel)
{
}

void CChannel::Reset_KeyFrame()
{
	m_iCurrentKeyFrame = 0;
}

HRESULT CChannel::SetUp_HierarchyNodePtr(CModel * pModel)
{
	m_pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	//Safe_AddRef(m_pHierarchyNode);

	return S_OK;
}

CChannel * CChannel::Create(aiNodeAnim * pAIChannel, CModel* pModel, DWORD* dwByte, HANDLE hFile)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel, dwByte, hFile)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Create(HANDLE hFile, DWORD* dwByte, CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(hFile, dwByte, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel * CChannel::Clone(CModel* pModel)
{
	CChannel*			pInstance = new CChannel(*this);

	if (FAILED(pInstance->SetUp_HierarchyNodePtr(pModel)))
		return nullptr;

	return pInstance;
}

void CChannel::Free()
{
	//Safe_Release(m_pHierarchyNode);
}
