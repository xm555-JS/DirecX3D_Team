#include "..\Public\Animation_Manager.h"

#include "Animation.h"
#include "Channel.h"

IMPLEMENT_SINGLETON(CAnimation_Manager);

CAnimation_Manager::CAnimation_Manager()
{
}

HRESULT CAnimation_Manager::Reserve_Container(_uint iNumGroup)
{
	if (nullptr != m_AnimationsGroup ||
		nullptr != m_Animations)
		__debugbreak();

	m_iNumGroup = iNumGroup;

	m_AnimationsGroup = new ANIMATIONSGROUP[iNumGroup];
	m_Animations = new ANIMATIONS[iNumGroup];

	return S_OK;
}

HRESULT CAnimation_Manager::Load_Animation(const char* pModelFilePath, _uint iAnimGroup, _uint iAnimState, STYLE eStyle)
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

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(hFile, &dwByte);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations[iAnimGroup].push_back(pAnimation);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CAnimation_Manager::Load_Animation(const char* pModelFilePath, _uint iAnimGroup)
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

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(hFile, &dwByte);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations[iAnimGroup].push_back(pAnimation);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CAnimation_Manager::Add_Animation(_uint iNumGroup, CAnimation* Animations)
{
	if (nullptr == Animations)
		__debugbreak();

	m_Animations[iNumGroup].push_back(Animations);

	return S_OK;
}

HRESULT CAnimation_Manager::Add_AnimationGroup(_uint iNumGroup, _uint iAnimState, vector<class CAnimation*> Animations)
{
	if (iNumGroup >= m_iNumGroup ||
		nullptr == m_AnimationsGroup)
		__debugbreak();

	m_AnimationsGroup[iNumGroup].emplace(iAnimState, Animations);

	return S_OK;
}

//vector<class CAnimation*> CAnimation_Manager::Get_Animations(_uint iNumGroup, _uint iAnimState)
//{
//	if (iNumGroup >= m_iNumGroup ||
//		nullptr == m_AnimationsGroup)
//		__debugbreak();
//
//	for (auto& iter : m_AnimationsGroup[iNumGroup])
//	{
//		if (iter.first == iAnimState)
//		{
//			m_Animations = iter.second;
//			break;
//		}
//	}
//
//	return m_Animations;
//}
//
//HRESULT CAnimation_Manager::SetUp_HierarchyNodePtr(_uint iAnimGroup, CModel* pModel)
//{
//	for (auto& iter : m_AnimationsGroup[iAnimGroup])
//	{
//		for (auto& Animation : iter.second)
//		{
//			Animation->SetUp_HierarchyNodePtr(pModel);
//		}
//	}
//
//	return S_OK;
//}

HRESULT CAnimation_Manager::SetUp_HierarchyNodePtr(_uint iAnimGroup, CModel* pModel)
{
	for (auto& Animation : m_Animations[iAnimGroup])
	{
		Animation->SetUp_HierarchyNodePtr(pModel);
	}

	return S_OK;
}

void CAnimation_Manager::Free()
{
	for (_uint i = 0; i < m_iNumGroup; ++i)
	{
		for (auto& pAnimation : m_AnimationsGroup[i])
		{
			for (auto& iter : pAnimation.second)
				Safe_Release(iter);
			pAnimation.second.clear();
		}

		m_AnimationsGroup[i].clear();

		for (auto& pAnimation : m_Animations[i])
		{
			Safe_Release(pAnimation);
		}

		m_Animations[i].clear();
	}

	Safe_Delete_Array(m_AnimationsGroup);
	Safe_Delete_Array(m_Animations);
}
