#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CWater_Base final : public CGameObject
{
public:
	CWater_Base();
	CWater_Base(const CWater_Base& rhs);
	virtual ~CWater_Base() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;


private:
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

	HRESULT		Setup_TransformState();

	void		Move_WaterSurface(_float fTimeDelta);

public:
	static CWater_Base* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END