#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Menu final : public CLevel
{
public:
	CLevel_Menu();
	virtual ~CLevel_Menu() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:	
	HRESULT Ready_Layer_BackGround(ID_UINT iLayerID);

public:
	static CLevel_Menu* Create();
	virtual void Free() override;
};

END