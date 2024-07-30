
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_MaskTexture;
vector g_vDiffuseTexture;
float g_fUVScaleXCur;
float g_fUVScaleYCur;
float g_fUVScaleXMax;
float g_fUVScaleYMax;

float4 g_vFlag;

bool g_bIsReverseXY_Mask;
bool	g_bGlow = false;
float4	g_vGlowColor;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    //vector vNormal : SV_TARGET1;
    //vector vDepth : SV_TARGET2;
    vector vGlow : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vTexUV_Mask;
    vTexUV_Mask.x = In.vTexUV.x / g_fUVScaleXMax + (g_fUVScaleXCur / g_fUVScaleXMax);
    vTexUV_Mask.y = In.vTexUV.y / g_fUVScaleYMax + (g_fUVScaleYCur / g_fUVScaleYMax);
    
    float2 temp;
    if (g_bIsReverseXY_Mask)
    {
        temp = vTexUV_Mask.x;
        vTexUV_Mask.x = vTexUV_Mask.y;
        vTexUV_Mask.y = temp;
    }
    
    vector g_Mask = g_MaskTexture.Sample(PointClampSampler, vTexUV_Mask);
    
    Out.vDiffuse.a = g_vDiffuseTexture.a * max(max(g_Mask.r, g_Mask.g), g_Mask.b);
    if (Out.vDiffuse.a < 0.01f)
        discard;
    
    Out.vDiffuse.rgb = g_vDiffuseTexture.rgb * g_Mask.rgb;
    
	if (true == g_bGlow)
	{
		Out.vGlow.rgb = g_vGlowColor.rgb;
        Out.vGlow.a = Out.vDiffuse.a * g_vGlowColor.a;
    }
    
    return Out;
}



technique11 DefaultTechnique
{
    pass DefaultSampler
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Cloud
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}