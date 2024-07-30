#pragma once

#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Client)
class CCursor;
class CEnemy_Boss abstract : public CEnemy
{
public:
	CEnemy_Boss();
	CEnemy_Boss(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Create_UI_Status(string strFileName);
	void	Set_StartCamAction() { m_bIsStartCamAction = true; }

protected:
	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };
	_bool	m_bIsStartCamAction = false;


protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual	HRESULT SetUp_State_Handler() = 0;

	void	Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform);

public:
	virtual void Free() override;
};

END