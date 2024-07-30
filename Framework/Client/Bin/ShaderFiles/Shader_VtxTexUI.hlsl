
#include "Client_Shader_Defines.hpp"
#include "Client_Shader_Function.hlsl"


matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float2		g_vSizeRatio = { 0.f,0.f };
float		g_fProgressRatio = 0.f;
float		g_fAlphaRatio = 1.f;
float		g_fFar = 80.f;

float		g_fScale = 1.f;
float2		g_vUVNum = { 1.f,1.f };
float2		g_vMoveUVSpeed = { 0.f,0.f };

float3		g_vColor = { 1.f,1.f,1.f };

bool		g_bDecrease = true;
bool		g_bGlow = false;
bool		g_bEdgeFade = false;
float       g_fGlowPower = 0.f;
float		g_fBright = 1.f;

float		g_fMapRange = 0.05f;
float2		g_vMapPos = { 0.f,0.f };

texture2D	g_DiffuseTexture;


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

struct PS_OUT_EFFECT
{
    vector vColor : SV_TARGET0;
    vector vGlow : SV_TARGET1;
};

struct PS_OUT_DEPTH
{
	vector		vColor : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
};

PS_OUT_EFFECT PS_MAIN(PS_IN In)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fBright;

	/*if (Out.vColor.a < 0.1f)
		discard;*/
	if (g_bGlow)
	{
        Out.vGlow += Out.vColor - g_fGlowPower;
    }

	return Out;	
}

PS_OUT_EFFECT PS_FADE(PS_IN In)
{
	PS_OUT_EFFECT Out = (PS_OUT_EFFECT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
    
	/*if (Out.vColor.a < 0.1f)
	discard;*/
	if (g_bGlow)
	{
		Out.vGlow += Out.vColor - g_fGlowPower;
	}
	Out.vColor.a *= g_fAlphaRatio;

	//if (g_bEdgeFade)
	//{
	//	/*if (0.95f < In.vTexUV.x)
	//	{
	//		Out.vColor.a *= (In.vTexUV.x - 1.f) * -20.f;
	//	}*/
	//	if (0.95f < In.vTexUV.y)
	//	{
	//		Out.vColor.a *= (In.vTexUV.y - 1.f) * -20.f;
	//	}
	//	if (0.05f > In.vTexUV.x)
	//	{
	//		Out.vColor.a *= In.vTexUV.x * 20.f;
	//	}
	//	/*if (0.05f > In.vTexUV.y)
	//	{
	//		Out.vColor.a *= In.vTexUV.y * 20.f;
	//	}*/
	//}

	return Out;
}

PS_OUT PS_UVMULTIPLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x * g_vUVNum.x, In.vTexUV.y * g_vUVNum.y));

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT_EFFECT PS_UVDISCARD(PS_IN In)
{
	PS_OUT_EFFECT Out = (PS_OUT_EFFECT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_bGlow)
	{
		Out.vGlow += Out.vColor - g_fGlowPower;
	}

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;

	if (In.vTexUV.y <= g_vSizeRatio.y)
		discard;

	return Out;
}

PS_OUT PS_UISTART(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (/*Out.vColor.a < 0.5f &&*/ Out.vColor.a > 0.1f)
	{
		Out.vColor.xyz = float3(0.996f,0.905f,0.96f);
	}

	return Out;
}

PS_OUT PS_PLAYER_HPBACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_bDecrease)
		Out.vColor.xyz = float3(0.9f,0.9f,0.9f );

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;

	return Out;
}

PS_OUT PS_PLAYER_HPBAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float		fGreenRatio = 1.f;
	float		fRedRatio = 0.f;

	if (0.5f < g_vSizeRatio.x)
	{
		fRedRatio = (1.f - g_vSizeRatio.x) * 2.f;
		fGreenRatio = 1.f;
	}
	else if (0.5f >= g_vSizeRatio.x)
	{
		fGreenRatio = (g_vSizeRatio.x * 2.f);
		fRedRatio = 1.f;
	}

	//Out.vColor.xyz = float3(0.8f, 0.f, 0.1f) * fRedRatio + float3(0.f, 0.8f, 0.1f) * fGreenRatio;
	Out.vColor.xyz = float3(0.8f, 0.f, 0.1f) * fRedRatio + float3(0.572f, 0.88f, 0.403f) * fGreenRatio;

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;


	return Out;
}

PS_OUT PS_SKILL_GAUGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.5f >= g_vSizeRatio.x)
	{
		if (0.5f > In.vTexUV.x)
			discard;

		if (In.vTexUV.y >= g_vSizeRatio.x * 2.f)
			discard;
	}
	else if (0.5f < g_vSizeRatio.x)
	{
		if (In.vTexUV.y <= g_vSizeRatio.x * -2.f + 2.f && 0.5f > In.vTexUV.x)
			discard;
	}

	return Out;
}

PS_OUT PS_STAMINA_BACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if(g_bDecrease)
		Out.vColor.xyz = float3(1.f, 0.156f, 0.18f);
	else
		Out.vColor.xyz = float3(0.27f, 0.886f, 0.423f);

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;

	if (In.vTexUV.y <= g_vSizeRatio.y)
		discard;
	
    Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_STAMINA_BAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.xyz = float3(0.69f, 0.99f, 0.823f);

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;

	if (In.vTexUV.y <= g_vSizeRatio.y)
		discard;
	
    Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_UVDISCARD_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (In.vTexUV.x >= g_vSizeRatio.x)
		discard;

	if (In.vTexUV.y <= g_vSizeRatio.y)
		discard;

	Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_UVPLUS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x + g_vMoveUVSpeed.x, In.vTexUV.y + g_vMoveUVSpeed.y));

	/*if (Out.vColor.a < 0.1f)
	discard;*/

	//Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT_EFFECT PS_ENEMY_INDICATOR(PS_IN In)
{
	PS_OUT_EFFECT Out = (PS_OUT_EFFECT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.xyz = float3(1.f, 0.2f, 0.2f);

    Out.vColor.a *= g_fAlphaRatio;

	if (g_bGlow)
	{
		Out.vGlow += Out.vColor - g_fGlowPower;
	}

	if (Out.vColor.a < 0.2f)
		discard;

	return Out;
}

PS_OUT PS_COLOR_CUSTOM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.5f > Out.vColor.x)
	{
		//Out.vColor.a = 1.f;
		Out.vColor.xyz = g_vColor;
	}

	Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_BRIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fBright;

	Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_MAINMENU(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_COLORFADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fBright;

	Out.vColor.rgb *= g_fBright;

	//Out.vColor.a *= g_fAlphaRatio;

	return Out;
}

PS_OUT PS_CIRCLE_PROGRESS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//g_fProgressRatio
	float2 vpos = In.vTexUV - 0.5f;
	float fAngle = degrees(atan2(-vpos.x, vpos.y)) + 180.f;
	float fa = radians(fAngle - g_fProgressRatio * 360.f) * 10000.f;

	fa = saturate(fa);

	if (fa > 0.f)
	{
		discard;
	}

	return Out;
}

PS_OUT PS_MINIMAP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/*float fRange = 0.1f;

	In.vTexUV.x = (In.vTexUV.x - g_vMapPos.x + fRange) / (2.f * fRange);
	In.vTexUV.y = (In.vTexUV.y - g_vMapPos.y + fRange) / (2.f * fRange);*/
	//In.vTexUV.x = ((In.vTexUV.x * 5.f) - (g_vMapPos.x * 5.f - 0.5f));
	//In.vTexUV.y = ((In.vTexUV.y * 5.f) - (g_vMapPos.y * 5.f - 0.5f));

	//float fRange = 0.7f;
	float2 vUV = g_vMapPos + (In.vTexUV.xy - 0.5f) * g_fMapRange;

	//0, 1;
	//(0, 0) = g_vMapPos - fRange;
	//(1, 1) = g_vMapPos + fRange;

	//fRange;
	//g_vMapPos.x - fRange;// 0
	//g_vMapPos.x + fRange;// 1

	//g_vMapPos.y - fRange;// 0
	//g_vMapPos.y + fRange;// 1

	/*if (0.f > In.vTexUV.x || 1.f < In.vTexUV.x || 0.f > In.vTexUV.y || 1.f < In.vTexUV.y)
		discard;*/

	Out.vColor = g_DiffuseTexture.Sample(LinearClampSampler, vUV);

	Out.vColor.a *= 0.55f;


	if (0.95f < In.vTexUV.x)
	{
		Out.vColor.a *= (In.vTexUV.x - 1.f) * -20.f + 0.15f;
	}
	if (0.95f < In.vTexUV.y)
	{
		Out.vColor.a *= (In.vTexUV.y - 1.f) * -20.f + 0.15f;
	}
	if (0.05f > In.vTexUV.x)
	{
		Out.vColor.a *= In.vTexUV.x * 20.f + 0.15f;
	}
	if (0.05f > In.vTexUV.y)
	{
		Out.vColor.a *= In.vTexUV.y * 20.f + 0.15f;
	}

	if (0.55f < Out.vColor.a)
	{
		Out.vColor.a = 0.55f;
	}

	
	return Out;
}


BlendState BS_UI_Glow
{
    BlendEnable[0] = true;
	//BlendEnable[1] = true;
    SrcBlend[0] = src_alpha;
    DestBlend[0] = inv_src_alpha;
    BlendOp[0] = add;
    BlendOpAlpha[0] = max;

    BlendEnable[1] = true;
	//BlendEnable[1] = true;
    SrcBlend[1] = src_alpha;
    DestBlend[1] = dest_alpha;
    BlendOp[1] = max;
    BlendOpAlpha[1] = add;
};

technique11 DefaultTechnique
{
	pass Default0
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass FadeInOut1
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_FADE();
	}
	pass UVMultiple2
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UVMULTIPLE();
	}
	pass UVDiscard3
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UVDISCARD();
	}
	pass UIStart4
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UISTART();
	}
	pass Player_HpBack5
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_PLAYER_HPBACK();
	}
	pass Player_HpBar6
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_PLAYER_HPBAR();
	}
	pass Skill_Gauge7
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SKILL_GAUGE();
	}
	pass Stamina_Back8
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_STAMINA_BACK();
	}
	pass Stamina_Bar9
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_STAMINA_BAR();
	}
	pass UVDiscard_Fade10
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UVDISCARD_FADE();
	}
	pass UVPlus11
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_UVPLUS();
	}
	pass Enemy_Indicator12
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_ENEMY_INDICATOR();
	}
	pass ColorCustom13
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COLOR_CUSTOM();
	}
	pass Bright14
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BRIGHT();
	}
	pass MainMenu15
	{
		SetBlendState(BS_UI_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINMENU();
	}
	pass CircleProgress16
	{
		SetBlendState(BS_BlendAlphaMax, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_CIRCLE_PROGRESS();
	}
	pass PS_ColorFade17
	{
		SetBlendState(BS_BlendAlphaMax, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORFADE();
	}
	pass PS_Minimap18
	{
		SetBlendState(BS_BlendMax, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MINIMAP();
	}
}