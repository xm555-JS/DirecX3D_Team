#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CState_Handler;

class CNpc abstract : public CGameObject
{
public:
	CNpc();
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_EventMessege(EVENTDESC tEvent);

protected:
	CCollider* m_pCol_Sphere = nullptr;
	CState_Handler* m_pState_Handler = nullptr;
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;

protected:
	void  Load_UI(string strFileName);

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	virtual HRESULT SetUp_State_Handler() PURE;

// 대화창 생성 관련
protected:
	list<class CUI*>	m_TalkWindowlist;
	list<class CUI*>	m_GuideWindow_Talklist;

protected:
	void Create_Talk_Window(OBJECT eType,wstring strName, wstring strTalkText);
	void Delete_Talk_Window();
	void Create_UI_GuideWindow_Talk(wstring strText);
	void Delete_UI_GuideWindow_Talk();

protected:
	_uint				m_iNumMeshContainers = 0;

private:
	wstring				m_strPreScript = L"";
	_bool				m_bOnce = false;

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; };
	virtual void Free() override;
};

END