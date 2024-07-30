#pragma once
#include "Base.h"
#include "Animation.h"

BEGIN(Engine)

class CAnimationManager final : public CBase
{
	DECLARE_SINGLETON(CAnimationManager);

public:
	CAnimationManager();
	virtual ~CAnimationManager() = default;

public:
	HRESULT Reserve_Container(_uint iNumGroup);
	HRESULT Add_AnimationGroup(_uint iNumGroup, _uint iAnimState, vector<class CAnimation*> Animations);
	vector<class CAnimation*> Get_Animations(_uint iNumGroup, _uint iAnimState);

private:
	_uint									m_iNumGroup;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

	map<_uint, ANIMATIONS>*					m_AnimationsGroup = nullptr;
	typedef map<_uint, ANIMATIONS>			ANIMATIONSGROUP;

private:
	_uint									m_iCurrentAnimGroupIndex = 0;
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iPreAnimationIndex = 0;
	_float									m_fTimeAcc = 0.f;
	_float									m_fTimeRatio = 0.1f;

public:
	virtual void Free() override;

};

END