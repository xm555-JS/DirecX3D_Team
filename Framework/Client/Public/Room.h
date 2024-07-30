#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CRoom final : public CGameObject
{
public:
	CRoom();
	CRoom(const CRoom& rhs);
	virtual ~CRoom() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	void	Input_Key(_float fTimeDelta);

	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;


	_uint m_iNumMeshContainers = 0;

public:
	static CRoom* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END