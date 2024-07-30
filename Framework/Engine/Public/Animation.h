#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	_bool Get_IsFinished() { return m_isFinished; }
	char* Get_AnimName() { return m_szName; }
	_float Get_Duration() { return m_fDuration; }
	_float Get_TickperSecond() { return m_fTickPerSecond; }
	_float Get_AccTime() { return m_fTimeAcc; }

	vector<class CChannel*> GetChannel();
	map<const char*, KEYFRAME> GetKeyframes();

	void Set_ChangeAnim() { m_isChanged = true; }
	void Set_EndFrame(_float fDuration);
	void Set_AccTime(_float fTime);
	void Set_KeyFrame(_uint iFrame);
	void Set_RootBoneName(string strBoneName) { m_strBoneName = strBoneName; }

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel, DWORD* dwByte, HANDLE hFile);
	HRESULT Initialize(HANDLE hFile, DWORD* dwByte, class CModel* pModel);
	HRESULT Initialize(HANDLE hFile, DWORD* dwByte);
	void Update_TransformationMatrices(_float fTimeDelta);
	void Update_TransformationMatrices(class CChannel* pChannel, _fmatrix TransformationMatrix);

public:
	HRESULT Clone_Channel(CAnimation* pPrototype, class CModel* pModel);
	void	Reset_KeyFrame();

public:
	HRESULT SetUp_HierarchyNodePtr(class CModel* pModel);

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* 현재 애니메이션이 사용ㅇ해야할 뼈의 갯수. */
	vector<class CChannel*>		m_Channels; /* 현재 애님에서 사용되는 뼈의 상태를 저장한 객체이다. */

	_float			m_fDuration = 0.f;
	_float			m_fEndDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fTimeAcc = 0.f;
	_bool			m_isFinished = false;
	_bool			m_isAnimFinished = false;
	_bool			m_isChanged = false;
	string			m_strBoneName = "";

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel, DWORD* dwByte = nullptr, HANDLE hFile = nullptr);
	static CAnimation* Create(HANDLE hFile, DWORD* dwByte, class CModel* pModel);
	static CAnimation* Create(HANDLE hFile, DWORD* dwByte);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END