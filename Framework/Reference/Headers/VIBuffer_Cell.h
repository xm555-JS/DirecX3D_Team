#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell final : public CVIBuffer
{
protected:
	CVIBuffer_Cell();
	CVIBuffer_Cell(const CVIBuffer_Cell& rhs);
	virtual ~CVIBuffer_Cell() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((_ushort*)m_pIndices)[0],
			((_ushort*)m_pIndices)[1],
			((_ushort*)m_pIndices)[2]);
	}

public:
	virtual HRESULT Initialize_Prototype(const _float3* pVerticesPos);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Cell* Create(const _float3 * pVerticesPos);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END