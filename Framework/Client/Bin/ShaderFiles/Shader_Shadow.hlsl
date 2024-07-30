
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fFar;

float		g_fPower = 1.f;
float		g_fRange = 1.f;

int			g_iNumLights = 1.f;


matrix		g_CamViewMatrixInv, g_CamProjMatrixInv;
matrix		g_LightViewMatrix, g_LightProjMatrix;

texture2D	g_DepthTexture;
texture2D	g_LightDepthTexture;


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


PS_OUT PS_MAIN_SHADOW(PS_IN In)
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


	float2		vLightUV;
	vWorldPos = mul(vWorldPos, g_LightViewMatrix);
	vWorldPos = mul(vWorldPos, g_LightProjMatrix);

	vWorldPos /= vWorldPos.w;

	float		fTargetDepth = vWorldPos.z;

	vLightUV.x = (vWorldPos.x + 1.f) * 0.5f;
	vLightUV.y = (vWorldPos.y - 1.f) * -0.5f;

	float		fBias = 0.003f;
	if (1.f - fBias < vLightUV.x ||
		1.f - fBias < vLightUV.y ||
		0.f + fBias > vLightUV.x ||
		0.f + fBias > vLightUV.y)
		discard;


	fTargetDepth -= fBias / g_fFar;

    vector      vLightDepth = g_LightDepthTexture.Sample(LinearSampler, vLightUV);
    if (1.f == vLightDepth.y)
        discard;

    if (fTargetDepth > vLightDepth.x)
	{
		//// real Z
		//fTargetDepth *= g_LightDepthTexture.Sample(LinearSampler, vLightUV).y * g_fFar;
		//float fAtt = fTargetDepth / g_fRange;

		//Out.vColor.r += (1.f - fAtt) / g_iNumLights * g_fPower;
        
        // real Z
        Out.vColor.r = 0.2f;
        //Out.vColor.r = 0.1f;
    }
    else
        discard;

    return Out;
}


PS_OUT PS_MAIN_SHADOW_DIR(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
    float fViewZ = vDepth.y * g_fFar;

    if (0 == fViewZ)
        discard;

    vector vWorldPos;

    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.0f;

    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_CamProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_CamViewMatrixInv);


    float2 vLightUV;
    vWorldPos = mul(vWorldPos, g_LightViewMatrix);
    vWorldPos = mul(vWorldPos, g_LightProjMatrix);

    vWorldPos /= vWorldPos.w;

    float fTargetDepth = vWorldPos.z;

    vLightUV.x = (vWorldPos.x + 1.f) * 0.5f;
    vLightUV.y = (vWorldPos.y - 1.f) * -0.5f;

    float fBias = 0.0003f;
    if (1.f - fBias < vLightUV.x ||
		1.f - fBias < vLightUV.y ||
		0.f + fBias > vLightUV.x ||
		0.f + fBias > vLightUV.y)
        discard;


    fTargetDepth -= fBias;

    float fLightDepth = g_LightDepthTexture.Sample(LinearSampler, vLightUV).x;


    if (fTargetDepth > fLightDepth)
    {
		// real Z
        fTargetDepth *= g_LightDepthTexture.Sample(LinearSampler, vLightUV).y * g_fFar;
        float fAtt = fTargetDepth / g_fRange;

        Out.vColor.r += (1.f - fAtt) / g_iNumLights * g_fPower;
    }



    return Out;
}

technique11 DefaultTechnique
{
    pass Shadow
    {
        //SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

    pass Shadow_Dir
    {
        SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW_DIR();
    }

}