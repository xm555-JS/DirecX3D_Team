
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fFar;

//float       g_fDiffusePower = 1.f;
float       g_fDiffusePower = 1.f;
float       g_fAlphaPower = 1.f;
float4      g_vMaxPower;
//float       g_fMaxAlphaPower = 1.f;


float		g_fRange = 1.f;

float       g_fYDepth = 0.f;

matrix		g_CamViewMatrixInv, g_CamProjMatrixInv;

texture2D	g_DepthTexture;

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

VS_OUT VS_MAIN_WATER(VS_IN In)
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


PS_OUT PS_MAIN_WATER(PS_IN In)
{

	PS_OUT			Out = (PS_OUT)0;

	vector			vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	float			fViewZ = vDepth.y * g_fFar;

	if (0 == fViewZ)
		discard;

	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepth.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;
	vWorldPos = mul(vWorldPos, g_CamProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_CamViewMatrixInv);


    if (vWorldPos.y < g_fYDepth)
	{
        Out.vColor.r = clamp((g_fYDepth - vWorldPos.y) * g_fDiffusePower, 0.f, g_vMaxPower.r);
        Out.vColor.g = clamp((g_fYDepth - vWorldPos.y) * g_fDiffusePower, 0.f, g_vMaxPower.g);
        Out.vColor.b = clamp((g_fYDepth - vWorldPos.y) * g_fDiffusePower, 0.f, g_vMaxPower.b);
        Out.vColor.a = clamp((g_fYDepth - vWorldPos.y) * g_fAlphaPower, 0.f, g_vMaxPower.a);
    }

	return Out;
}



technique11 DefaultTechnique
{
    pass Water
    {
        //SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_WATER();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WATER();
    }

}