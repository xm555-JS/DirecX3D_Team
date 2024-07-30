#pragma once

#include "Client_Defines.h"
#include "Loader.h"

class CLoader_Edit_UI final : public CLoader
{
public:
	CLoader_Edit_UI();
	virtual ~CLoader_Edit_UI() = default;

public:
	HRESULT	Initialize();

public:
	virtual HRESULT Loading_Object() override;
	virtual HRESULT Loading_Texture() override;
	virtual HRESULT Loading_Model() override;
	virtual HRESULT Loading_Shader() override;

public:
	static CLoader_Edit_UI* Create();
	virtual void Free() override;
};

