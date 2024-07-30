#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CEffect_Factory;
class CEffect_Base;
// Effect
class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
	typedef map<const string, class CEffect_Factory*> EFFECTS;

public:
	enum FOLLOW_MODE { FOLLOW_POS, FOLLOW_POS_RUL, FOLLOW_MODE_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT				Load_EffectsData(string strFileName, EFFECTS* mapEffects);
	HRESULT				Save_EffectsData(string strFileName, EFFECTS* mapEffects);
	HRESULT				Load_EffectsFile(string strFileName);
	HRESULT				Save_EffectsFile(string strFileName);
	HRESULT				ExcuteEffect(const string& EffectName, _matrix matTransform, _float fPlaySpeed = 1.f, CGameObject* ParentObject = nullptr, FOLLOW_MODE eFollowMode = FOLLOW_MODE_END, string pBoneName = "", list<CEffect_Base*>* pOutList = nullptr);
	HRESULT				Add_Effect(const string& EffectName, vector<MESHEFFECT_DESC>& MeshEffectDesc, vector<PARTICLE_DESC>& ParticleDesc);
	HRESULT				Delete_Effect(const string& EffectName);
	HRESULT				Update_EffectDataFile(void);
	map<const string, class CEffect_Factory*> Get_Effects(void) { return m_Effects; }

private:
	class CEffect_Factory* Find_Effect(const string& EffectName, EFFECTS* pEffects = nullptr);

private:
	EFFECTS			m_Effects;
	EFFECTS			m_Effects_InAllFiles;

public:
	void Create_Effect_Positioning(string strEffectName, _float4x4* matParent, _fvector vPos = XMVectorSet(0.f, 0.f, 0.f, 1.f), _fvector vRotation = XMVectorSet(0.f, 0.f, 0.f, 0.f), _float fPlaySpeed = 1.f, _fvector vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f), _fvector vRevolution = XMVectorSet(0.f, 0.f, 0.f, 0.f), list<CEffect_Base*>* pOutList = nullptr);
	void Create_Effect_Positioning_Rotaion(string strEffectName, _float4x4* matParent, _fvector vPos = XMVectorSet(0.f, 0.f, 0.f, 1.f), _matrix matRotation = XMMatrixIdentity(), _float fPlaySpeed = 1.f, _fvector vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f), _fvector vRevolution = XMVectorSet(0.f, 0.f, 0.f, 0.f), list<CEffect_Base*>* pOutList = nullptr);

public:
	virtual void Free();

};
END