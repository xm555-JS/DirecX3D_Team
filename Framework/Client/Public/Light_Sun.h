#pragma once

#include "Client_Defines.h"
#include "Light.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END


BEGIN(Client)

class CLight_Sun : public CLight
{
private:
	CLight_Sun();
	virtual ~CLight_Sun() = default;

public:
	virtual HRESULT Initialize(const LIGHTDESC& LightDesc) override;
	virtual void	Tick(_float fTimeDelta) override {};
	virtual HRESULT Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer) override;
	virtual HRESULT Render_Shadows(CShader* pShader, CVIBuffer_Rect* pVIBuffer) override;

private:
	DECL_ONCE(m_bSun);

public:
	static CLight_Sun* Create(const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END