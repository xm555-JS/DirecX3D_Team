#pragma once

#include "Client_Defines.h"
#include "Loader.h"

class CLoader_Logo final : public CLoader
{
public:
	CLoader_Logo();
	virtual ~CLoader_Logo() = default;

public:
	HRESULT	Initialize();

public:
	virtual HRESULT Loading_Object() override;
	virtual HRESULT Loading_Texture() override;
	virtual HRESULT Loading_Model() override;
	virtual HRESULT Loading_Shader() override;

private:
	HRESULT Loading_UI_Object();
	HRESULT Loading_UI_Texture();

public:
	static CLoader_Logo* Create();
	virtual void Free() override;
};

