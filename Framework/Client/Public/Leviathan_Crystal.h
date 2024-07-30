#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"

//BEGIN(Engine)
//class CShader;
//class CRenderer;
//class CModel;
//class CVIBuffer;
//class CTexture;
//class CPhyXCollider;
//class CCollider;
//class CState_Handler;
//END

BEGIN(Client)
class CEffect_Base;
class CLeviathan_Crystal final : public CEnemy_Normal
{
public:
	CLeviathan_Crystal();
	CLeviathan_Crystal(const CLeviathan_Crystal& rhs);
	virtual ~CLeviathan_Crystal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void	On_Dead();

private:
	HRESULT SetUp_Components();
	void	Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform);

private:
	list<CEffect_Base*>		m_Effects;
	_float m_fTimeAcc_Effect0 = 0.f;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

public:
	static CLeviathan_Crystal* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END