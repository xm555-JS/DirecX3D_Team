#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Edit_UI final : public CLevel
{
public:
	CLevel_Edit_UI();
	virtual ~CLevel_Edit_UI() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	Ready_Layer_Sky(ID_UINT iID);
	HRESULT Ready_Layer_BackGround(ID_UINT iLayerID);
	HRESULT	Ready_Layer_Camera(ID_UINT iID);


public:
	static CLevel_Edit_UI* Create();
	virtual void Free() override;
};

END