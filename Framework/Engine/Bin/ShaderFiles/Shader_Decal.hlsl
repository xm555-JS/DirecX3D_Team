
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


float		g_fFar = 80.f;

matrix		g_CamViewMatrixInv, g_CamProjMatrixInv;
matrix		g_DecalViewMatrix, g_DecalProjMatrix;

texture2D	g_DepthTexture;

texture2D	g_DecalTargetTexture;
texture2D	g_DecalDepthTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DECAL(PS_IN In)
{

	PS_OUT			Out = (PS_OUT)0;

	//vector			vTexture = g_DecalTargetTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	// 300? far
	float			fViewZ = vDepth.y * g_fFar;

	if (0 == fViewZ)
		discard;

	//if (vTexture.r == 1.f && vTexture.b == 1.f)
	//	discard;

	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepth.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;
	vWorldPos = mul(vWorldPos, g_CamProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_CamViewMatrixInv);


	float2		vDecalTexUV;
	//vector		vCamWorldpos = vWorldPos;
	vWorldPos = mul(vWorldPos, g_DecalViewMatrix);
	vWorldPos = mul(vWorldPos, g_DecalProjMatrix);



	//vWorldPos /= fViewZ;
	vWorldPos /= vWorldPos.w;

	float		fTargetDepth = vWorldPos.z;

	vDecalTexUV.x = (vWorldPos.x + 1.f) * 0.5f;
	vDecalTexUV.y = (vWorldPos.y - 1.f) * -0.5f;

	float		fBias = 0.0005f;
	if (1.f - fBias < vDecalTexUV.x ||
		1.f - fBias < vDecalTexUV.y ||
		0.f + fBias > vDecalTexUV.x ||
		0.f + fBias > vDecalTexUV.y)
		discard;


	Out.vColor = g_DecalTargetTexture.Sample(LinearSampler, vDecalTexUV);
	if (0.f == Out.vColor.a)
		discard;

	fTargetDepth = fTargetDepth - fBias;

	float		fDecalDepth = g_DecalDepthTexture.Sample(LinearSampler, vDecalTexUV).x;

	if (fTargetDepth > fDecalDepth)
		discard;

	//if (Out.vColor.a == 0.f)
	//	discard;

	return Out;
}





technique11 DefaultTechnique
{
	pass Decal
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DECAL();
	}
}