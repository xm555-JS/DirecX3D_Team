#pragma once

#include "Client_Defines.h"
#include "Light.h"

BEGIN(Engine)
class CTrasnform;
END


BEGIN(Client)

class CLight_Target : public CLight
{
private:
	CLight_Target();
	virtual ~CLight_Target() = default;

public:
	CGameObject*		Get_TargetObject() { return m_pTarget; }
	_fvector			Get_Offset() { return XMLoadFloat3(&m_vOffset); }

	void				Set_TargetObject(CGameObject* pTarget);
	void				Set_Offset(_fvector vOffset);

public:
	virtual void	Tick(_float fTimeDelta);
	virtual HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	virtual HRESULT Render_Shadows(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	CGameObject*		m_pTarget = nullptr;
	CTransform*			m_pTargetTransform = nullptr;
	_float3				m_vOffset = { 0.f, 0.f, 0.f };

private:
	void			Update_Target(CTransform* pTargetTransform);
	virtual void	Setup_ViewPort() override;


public:
	static CLight_Target* Create(const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END