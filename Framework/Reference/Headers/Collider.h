#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_RAY, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		tagColliderDesc()
		{
			ZeroMemory(this, sizeof(CCollider::COLLIDERDESC));
		}
		_float3			vScale;
		_float4			vRotation;
		_float3			vTranslation;
	}COLLIDERDESC;

private:
	CCollider();
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;
	
public:
	_bool			Get_ColliderState() { return m_bCollable; }
	COLLIDERDESC*	Get_ColliderDesc() { return &m_ColliderDesc; }
	BoundingSphere* Get_BoundingSphere() { return m_pSphere; }

	void			Set_ColliderState(_bool bState) { m_bCollable = bState; }

	void			Set_Scale(_fvector vScale);
	_vector			Get_Scale();
	void			Set_Position(_fvector vPos);
	_vector			Get_Position();

	void			Clear_Colliders();

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg);
	void	LateTick();

#ifdef _COLLIDER_RENDER
public:
	HRESULT Render();
#endif // _DEBUG

public:
	void Update(_fmatrix TransformMatrix);

	_bool Collision(CCollider* pTargetCollider);
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

	list<CCollider*>*	Get_EnterColliders() { return &m_pEnterColliders; };
	list<CCollider*>*	Get_StayColliders() { return &m_pStayColliders; };
	list<CCollider*>*	Get_ExitColliders() { return &m_pExitColliders; };

	//_float			Get_Radius() { return m_fRadius; }
	//const TYPE&		Get_ColliderType() { return m_eColliderType; }
	//const _float3	Get_Ray() { return m_vRay; }

	//void	Set_Ray(_float3 vRay) { m_vRay = vRay; }
	//void	Set_Radius(_float fRadius) { m_fRadius = fRadius; }

public:
	void OnCollision(CCollider* pOther);

private:
	BoundingSphere*				m_pSphere_Original = nullptr;
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingOrientedBox*		m_pOBB_Original = nullptr;

	BoundingSphere*				m_pSphere = nullptr;
	BoundingBox*				m_pAABB = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;


	TYPE						m_eType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;
	_bool						m_isColl = false;

	_bool						m_bCollable = true;

#ifdef _COLLIDER_RENDER
private:
	BasicEffect*									m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	ComPtr<ID3D11Device>							m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>						m_pContext = nullptr;
#endif // _DEBUG

private:
	//_float		m_fRadius = 0.f;
	//_float3		m_vRay = { 0.f, 0.f, 0.f };

	list<CCollider*>		m_pCurColliders;
	list<CCollider*>		m_pPreColliders;

	//TYPE				m_eColliderType = SPHERE;
	//COLLIDERDESC		m_ColliderDesc;
	list<CCollider*>		m_pEnterColliders;
	list<CCollider*>		m_pStayColliders;
	list<CCollider*>		m_pExitColliders;



public:
	static CCollider* Create(TYPE eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END