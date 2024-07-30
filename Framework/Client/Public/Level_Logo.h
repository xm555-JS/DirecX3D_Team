#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
public:
	CLevel_Logo();
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	Ready_Layer_Camera(ID_UINT iLayerID);
	HRESULT Ready_Layer_UI(ID_UINT iLayerID);

public:
	static CLevel_Logo* Create();
	virtual void Free() override;
};

END