#pragma once

#include "Client_Defines.h"
#include "Loader.h"

class CLoader_Menu final : public CLoader
{
public:
	CLoader_Menu();
	virtual ~CLoader_Menu() = default;

public:
	HRESULT	Initialize();

public:
	virtual HRESULT Loading_Object() override;
	virtual HRESULT Loading_Texture() override;
	virtual HRESULT Loading_Model() override;
	virtual HRESULT Loading_Shader() override;




public:
	static CLoader_Menu* Create();
	virtual void Free() override;
};

