
#include "Client_Shader_Defines.hpp"
#include "Client_Shader_Function.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float g_AlphaLate = 1.f;
float		g_fFar = 80.f;

float		g_fScale = 1.f;
float       g_fAlphaRate = 1.f;

bool		g_bIsInDoor = true;

texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_FinalTexture;
texture2D	g_DepthTexture;
texture2D	g_MaskDepthTexture;

texture2D   g_NoiseTexture_0;
texture2D   g_NoiseTexture_1;

float2      g_vUVSpeed_0;
float2      g_vUVSpeed_1;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

struct PS_OUT_DEPTH
{
	vector		vColor : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;	
}


PS_OUT PS_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	Out.vColor.a *= g_AlphaLate;

	return Out;
}


PS_OUT_DEPTH PS_CAPTURE_MASK(PS_IN In)
{
	PS_OUT_DEPTH		Out = (PS_OUT_DEPTH)0;

	//Out.vColor = g_FinalTexture.Sample(DefaultSampler, In.vTexUV);
	//vector	vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	//float	fViewZ = vDepthDesc.y * g_fFar;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	

	//if(In.vProjPos.z < vDepthDesc.x * vDepthDesc.y * g_fFar)
	Out.vColor.g = 1.f;

	//if (Out.vColor.a < 0.1f)
	//	discard;

	return Out;
}


PS_OUT PS_CAPTURE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	if (1.f != vMask.g)
		discard;
	
	vector	vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector	vMaskDepthDesc = g_MaskDepthTexture.Sample(DefaultSampler, In.vTexUV);
	
	if (true == g_bIsInDoor)
	{
		if (vMaskDepthDesc.x > vDepthDesc.x)
			discard;
	}
	else
	{
		if (vMaskDepthDesc.x < vDepthDesc.x)
			discard;
	}

	//In.vTexUV.x = In.vTexUV.x + In.vTexUV.x * (g_fScale - 1.f);
	//In.vTexUV.y = In.vTexUV.y + In.vTexUV.y * (g_fScale - 1.f);

	Out.vColor = g_FinalTexture.Sample(DefaultSampler, In.vTexUV);
	//Out.vColor.r = 1.f;

	//if (Out.vColor.a < 0.1f)
	//	discard;

	return Out;
}


PS_OUT PS_MAIN_CLOUD(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if (0.f < Out.vColor.a)
        Out.vColor.rgb = 1.f;
    
    //vector vNoise0 = g_NoiseTexture_0.Sample(DefaultSampler, In.vTexUV + g_vUVSpeed_0);
    //vector vNoise1 = g_NoiseTexture_1.Sample(DefaultSampler, In.vTexUV + g_vUVSpeed_1);
    
    Out.vColor.a = Out.vColor.a/* *  (vNoise0.r + vNoise1.r)*/ * g_fAlphaRate;
    
    
    if (Out.vColor.a < 0.05f)
        discard;

    return Out;
}

PS_OUT PS_MAIN_CLOUD_SKY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_fScale);

    Out.vColor.a = Out.vColor.r;
    Out.vColor.rgb = 1.f;
    
    if (Out.vColor.a < 0.05f)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass FadeInOut
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_FADE();
	}
	pass Cursor
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Capture_Mask
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_CAPTURE_MASK();
	}

	pass Capture
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_CAPTURE();
	}

	pass EndTimer
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    pass Cloud
    {
        SetBlendState(BS_BlendMax, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_Zero, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLOUD();
    }
    pass Cloud_Sky
    {
        SetBlendState(BS_BlendMax, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLOUD_SKY();
    }
	//pass Default
	//{
	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	GeometryShader = NULL;
    //  HullShader = NULL;
    //  DomainShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN();
	//}
}