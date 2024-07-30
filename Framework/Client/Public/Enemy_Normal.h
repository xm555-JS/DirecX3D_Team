#pragma once

#include "Client_Defines.h"
#include "Enemy.h"
#include "Imgui_Manager.h"

//BEGIN(Engine)
//class CShader;
//class CRenderer;
//class CModel;
//class CVIBuffer;
//class CTexture;
//class CPhyXCollider;
//class CCollider;
//
//END

BEGIN(Client)
//class CCursor;

class CEnemy_Normal abstract : public CEnemy
{
public:
	CEnemy_Normal();
	CEnemy_Normal(const CEnemy_Normal& rhs);
	virtual ~CEnemy_Normal() = default;

public:
	_bool Get_Turn() { return m_bTurn; }
	_bool Get_IsPlayer_InRange() { return m_bIsPlayer_InRange; }
	
	void Set_Turn(_bool bTurn) { m_bTurn = bTurn; }
	void Set_IsPlayer_InRange(_bool bIsIn) { m_bIsPlayer_InRange = bIsIn; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);

protected:
	virtual HRESULT SetUp_ShaderResource();

protected:
	HRESULT	Create_UI_Status(_uint iNameTexIdx, _float fDeltaY = 0.f, _bool bNamed = false);

protected:
	CCollider* m_pCol_Sphere[COL_TYPE_END];

public:
	virtual void Free() override;

#if defined(_TEST_JJ)

protected:
	void Set_AttackColliderSizePos(CCollider* _Collider)
	{
		//_float SetPosX = DEBUGFLOAT4(0).x;
		//_float SetPosY = DEBUGFLOAT4(0).y;
		//_float SetPosZ = DEBUGFLOAT4(0).z;
		//_float SetScale = DEBUGFLOAT4(0).w;
		//_Collider->Set_Position(XMVectorSet(SetPosX, SetPosY, SetPosZ, 1.f));
		//_Collider->Set_Scale(XMVectorSet(SetScale, SetScale, SetScale,1.f));
	}
	void DeadTestKey()
	{
		USEGAMEINSTANCE;
		if (KEY_DOWN(VK_NUMPAD5))
			m_pCurInfo->fHP -= 10.f;
	}
#endif

protected:
	_bool	m_bTurn = false;
	_bool	m_bIsPlayer_InRange = false;


};

END