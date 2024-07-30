#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
	
	
	vector<class CChannel*>		m_Channels; /* 현재 애님에서 사용되는 뼈의 상태를 저장한 객체이다. */

	
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_iNumChannels(rhs.m_iNumChannels)
	, m_fDuration(rhs.m_fDuration)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fTimeAcc(rhs.m_fTimeAcc)
	, m_isFinished(rhs.m_isFinished)
{
	strcpy_s(m_szName, rhs.m_szName);
}

vector<class CChannel*> CAnimation::GetChannel()
{
	vector<CChannel*> KeyFrame;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		KeyFrame.push_back(m_Channels[i]);
	}

	return KeyFrame;
}

map<const char*, KEYFRAME> CAnimation::GetKeyframes()
{
	map<const char*, KEYFRAME> KeyFrame;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		KeyFrame.emplace(m_Channels[i]->Get_ChannelName(), m_Channels[i]->Get_KeyFrame());
	}

	return KeyFrame;
}

void CAnimation::Set_EndFrame(_float fDuration)
{
	m_fEndDuration = fDuration;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Set_EndFrame(fDuration);
	}
}

HRESULT CAnimation::Initialize(aiAnimation * pAIAnimation, class CModel* pModel, DWORD* dwByte, HANDLE hFile)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);
	DWORD iLen = (DWORD)strlen(m_szName) + 1;
	WriteFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	WriteFile(hFile, m_szName, sizeof(char) * iLen, dwByte, nullptr);

	m_iNumChannels = pAIAnimation->mNumChannels;

	/* 현재 애니메이션을 재상하는데 걸리는 시간. */
	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	WriteFile(hFile, &m_iNumChannels, sizeof(_uint), dwByte, nullptr);
	WriteFile(hFile, &m_fDuration, sizeof(_float), dwByte, nullptr);
	WriteFile(hFile, &m_fTickPerSecond, sizeof(_float), dwByte, nullptr);

	for (_uint i = 0; i < m_iNumChannels; ++i)	
	{
		aiNodeAnim* pAIChannel = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pAIChannel, pModel, dwByte, hFile);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}	

	return S_OK;
}

HRESULT CAnimation::Initialize(HANDLE hFile, DWORD* dwByte, CModel* pModel)
{
	DWORD iLen;
	ReadFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	ReadFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

	ReadFile(hFile, &m_iNumChannels, sizeof(_uint), dwByte, nullptr);
	ReadFile(hFile, &m_fDuration, sizeof(_float), dwByte, nullptr);
	ReadFile(hFile, &m_fTickPerSecond, sizeof(_float), dwByte, nullptr);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(hFile, dwByte, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(HANDLE hFile, DWORD* dwByte)
{
	DWORD iLen;
	ReadFile(hFile, &iLen, sizeof(DWORD), dwByte, nullptr);
	ReadFile(hFile, &m_szName, sizeof(char) * iLen, dwByte, nullptr);

	ReadFile(hFile, &m_iNumChannels, sizeof(_uint), dwByte, nullptr);
	ReadFile(hFile, &m_fDuration, sizeof(_float), dwByte, nullptr);
	ReadFile(hFile, &m_fTickPerSecond, sizeof(_float), dwByte, nullptr);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(hFile, dwByte);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrices(_float fTimeDelta)
{
	m_fTimeAcc += m_fTickPerSecond * fTimeDelta;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if(m_strBoneName != "" && !strcmp(m_strBoneName.c_str(), m_Channels[i]->Get_ChannelName()))
			continue;

		m_Channels[i]->Update_TransformationMatrices(m_fTimeAcc);
	}

	if (0 != m_fEndDuration && m_fTimeAcc >= m_fEndDuration)
	{
		m_isFinished = true;
		m_fTimeAcc = 0.f;
	}
	else if (m_fTimeAcc >= m_fDuration)
	{
		m_isFinished = true;
		m_fTimeAcc = 0.f;
	}
}

void CAnimation::Set_AccTime(_float fTime)
{
	m_fTimeAcc = fTime;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Set_TimeCurrentKeyFrame(fTime);
	}
}

void CAnimation::Set_KeyFrame(_uint iFrame)
{
	//m_fTimeAcc = fTime;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Set_NumCurrentKeyFrame(iFrame, &m_fTimeAcc);
	}
}

void CAnimation::Update_TransformationMatrices(CChannel* pChannel, _fmatrix TransformationMatrix)
{
	pChannel->Set_HierarchyNodeMatrix(TransformationMatrix);
}

HRESULT CAnimation::Clone_Channel(CAnimation * pPrototype, CModel * pModel)
{
	for (auto& pPrototypeChannel : pPrototype->m_Channels)
	{
		m_Channels.push_back(pPrototypeChannel->Clone(pModel));
	}

	return S_OK;
}

void CAnimation::Reset_KeyFrame()
{
	m_fEndDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_isFinished = false;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Reset_KeyFrame();
	}
}

HRESULT CAnimation::SetUp_HierarchyNodePtr(CModel* pModel)
{
	for (auto& Channel : m_Channels)
	{
		Channel->SetUp_HierarchyNodePtr(pModel);
	}

	return S_OK;
}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, CModel* pModel, DWORD* dwByte, HANDLE hFile)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel, dwByte, hFile)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create(HANDLE hFile, DWORD* dwByte, CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(hFile, dwByte, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create(HANDLE hFile, DWORD* dwByte)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(hFile, dwByte)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Clone(CModel * pModel)
{
	CAnimation*		pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Clone_Channel(this, pModel)))
		return nullptr;

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}
