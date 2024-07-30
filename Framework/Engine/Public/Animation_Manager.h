#pragma once
#include "Base.h"
#include "Animation.h"

BEGIN(Engine)

class CAnimation_Manager final : public CBase
{
	DECLARE_SINGLETON(CAnimation_Manager);

public:
	CAnimation_Manager();
	virtual ~CAnimation_Manager() = default;

public:
	enum STYLE {MAP_STYLE, VECTOR_STYLE, STYLE_END};

public:
	HRESULT Reserve_Container(_uint iNumGroup);
	HRESULT	Load_Animation(const char* pModelFilePath, _uint iAnimGroup, _uint iAnimState, STYLE eStyle);
	HRESULT	Load_Animation(const char* pModelFilePath, _uint iAnimGroup);
	HRESULT Add_Animation(_uint iNumGroup, class CAnimation* Animations);
	HRESULT Add_AnimationGroup(_uint iNumGroup, _uint iAnimState, vector<class CAnimation*> Animations);
	//vector<class CAnimation*> Get_Animations(_uint iNumGroup, _uint iAnimState);
	vector<class CAnimation*> Get_Animations(_uint iNumGroup) { return m_Animations[iNumGroup]; }

public:
	HRESULT SetUp_HierarchyNodePtr(_uint iAnimGroup, class CModel* pModel);
	//HRESULT SetUp_HierarchyNodePtr(_uint iAnimGroup, class CModel* pModel);

private:
	_uint									m_iNumGroup;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>*				m_Animations = nullptr;
	typedef vector<class CAnimation*>		ANIMATIONS;

	map<_uint, ANIMATIONS>*					m_AnimationsGroup = nullptr;
	typedef map<_uint, ANIMATIONS>			ANIMATIONSGROUP;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:
	_uint									m_iCurrentAnimGroupIndex = 0;
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iPreAnimationIndex = 0;
	_float									m_fTimeAcc = 0.f;
	_float									m_fTimeRatio = 0.1f;

	STYLE									m_eStyle = STYLE_END;

public:
	virtual void Free() override;

};

END