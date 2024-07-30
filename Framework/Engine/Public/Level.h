#pragma once

#include "Base.h"

BEGIN(Engine)
class CCamera;

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel();
	virtual ~CLevel() = default;


public:
	virtual vector<CCamera*>* Get_FreeCameras(void) { return nullptr; }
	virtual	void Add_FreeCamera(void) {}




public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta) {};
	virtual void LateTick(_float TimeDelta) {};
	virtual HRESULT Render();

protected:
	ComPtr<ID3D11Device>			m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;

public:
	virtual void Free() override;

};

END