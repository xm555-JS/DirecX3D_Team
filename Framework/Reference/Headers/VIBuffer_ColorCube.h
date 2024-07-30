#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ColorCube final : public CVIBuffer
{
public:
	typedef struct ColorCubeDesc
	{
		_ulong			dwColor = 0xffffffff;
		_float3			vScale = { 0.f, 0.f, 0.f };
		_float3			vPos = { 0.f, 0.f, 0.f };
	}COLORCUBEDESC;

public:
	CVIBuffer_ColorCube();
	CVIBuffer_ColorCube(const CVIBuffer_ColorCube& Prototype);
	virtual ~CVIBuffer_ColorCube() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	void	Set_Color(_ulong dwColor) { m_dwColor = dwColor; }
	void	Set_Scale(_float3 vScale) { m_vScale = vScale; }


private:
	_ulong			m_dwColor = 0xfffffffff;
	_float3			m_vScale = { 0.f, 0.f, 0.f };
	_float3			m_vPos = { 0.f, 0.f, 0.f };
	COLORCUBEDESC	m_ColorCubeDesc;


public:
	static CVIBuffer_ColorCube* Create();
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END