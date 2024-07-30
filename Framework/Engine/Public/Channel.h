#pragma once

#include "Base.h"

/* 각 애니메이션마다 따로 할당되어 보관된다. */

BEGIN(Engine)
class CModel;

class ENGINE_DLL CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	void Set_KeyFrame(_uint iNumFrame) { m_iCurrentKeyFrame = iNumFrame; }
	vector<KEYFRAME> Get_KeyFrames();
	KEYFRAME Get_CurrentKeyFrame() { return m_KeyFrames[m_iCurrentKeyFrame]; }
	KEYFRAME Get_KeyFrame();
	char* Get_ChannelName() { return m_szName; }
	_uint Get_NumKeyframes() { return m_iNumKeyframes; }
	_uint Get_NumCurrentKeyFrame() { return m_iCurrentKeyFrame; }

	void Set_EndFrame(_float fEndDuration) { m_EndDuration = fEndDuration; }
	void Set_HierarchyNodeMatrix(_fmatrix TransformationMatrix);
	void Set_TimeCurrentKeyFrame(_float fTime);
	void Set_NumCurrentKeyFrame(_uint iKeyFrame, _float* fTime);
	void Set_HierarchyNode(CModel* pModel);

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, CModel* pModel, DWORD* dwByte, HANDLE hFile);
	HRESULT Initialize(HANDLE hFile, DWORD* dwByte, CModel* pModel);

	void Update_TransformationMatrices(_float fCurrentTime);
	void Reset_KeyFrame();

public:
	HRESULT SetUp_HierarchyNodePtr(class CModel* pModel);
private:
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrentKeyFrame = 0;
	KEYFRAME					m_KeyFrame;

	_float						m_EndDuration = 0.f;
	_bool						m_bUsedEndFrame = false;
	


public:
	static CChannel* Create(aiNodeAnim* pAIChannel, CModel* pModel, DWORD* dwByte = nullptr, HANDLE hFile = nullptr);
	static CChannel* Create(HANDLE hFile, DWORD* dwByte, CModel* pModel = nullptr);
	CChannel* Clone(CModel* pModel);

	virtual void Free() override;
};

END