#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Effect_Manager.h"

BEGIN(Client)
class CEffect_Base;
class CEffect_Factory final : public CBase
{
private:
	CEffect_Factory();
	virtual ~CEffect_Factory() = default;

public:
	HRESULT Initialize(const string _Effect_Name, vector<PARTICLE_DESC> _ParticleDesc, vector<MESHEFFECT_DESC> _MeshEffectDesc);
	HRESULT	Execute(_matrix matTransform, _float fTimeSpeed = 1.f, CGameObject* ParentObject = nullptr, CEffect_Manager::FOLLOW_MODE eFollowMode = CEffect_Manager::FOLLOW_MODE_END, string pBoneName = "", list<CEffect_Base*>* pOutList = nullptr);
	string&						Get_EffectDesc_EffectName(void) { return m_strEffect_Name; };
	vector<PARTICLE_DESC>&		Get_EffectDesc_Particle(void) { return m_listParticleDesc; };
	vector<MESHEFFECT_DESC>&	Get_EffectDesc_Mesh(void) { return m_listMeshEffectDesc; };

private:
	//EFFECT_DESC						m_EffectDesc;
	string						m_strEffect_Name;
	vector<PARTICLE_DESC>		m_listParticleDesc;
	vector<MESHEFFECT_DESC>		m_listMeshEffectDesc;

public:
	static CEffect_Factory* Create(const string _Effect_Name, vector<PARTICLE_DESC> _ParticleDesc, vector<MESHEFFECT_DESC> _MeshEffectDesc);
	virtual void Free() override;
};

END