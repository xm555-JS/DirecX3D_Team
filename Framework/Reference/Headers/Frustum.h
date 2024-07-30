#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum();
	virtual ~CFrustum() = default;
public:
	/* ������������ ����ü�� �����ϴ� ������ ���� ��� ���Ѵ�. */	
	HRESULT Initialize();

	/*���� �������� ������ ����� ���Ѵ�. */
	void	Tick();

public:
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0.f);
	_bool isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange = 0.f);
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);

private:
	_float4				m_vOriginalPoints[8];
	_vector				m_vWorldPoints[8];
	_float4				m_vWorldPlane[6];
	_float4				m_vLocalPlane[6];

public:
	virtual void Free() override;
};

END