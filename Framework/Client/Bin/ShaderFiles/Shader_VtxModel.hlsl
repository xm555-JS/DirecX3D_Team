
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix	g_DecalViewMatrix, g_DecalProjMatrix;
matrix	g_CamViewMatrixInv, g_CamProjMatrixInv;

texture2D	g_DiffuseTexture;
float		g_fDiffusePower = 1.f;
float		g_fSaturationRate = 1.f;

texture2D	g_NormalTexture;

vector		g_vCamPosition;

vector		g_vLightDir;
vector		g_vLightPos;
float		g_fRange = 1.f;

float		g_fFar;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 30.f;
float		g_fBrightness = 0.5f;
float		g_fBloomPower = 1.f;

float		g_fUVRange = 1.f;
float       g_fLineBrightness = 1.f;
float4      g_fOutlineColor = vector(1.f, 1.f, 1.f, 1.f);


float		g_CX = 1.f;
float		g_CZ = 1.f;


bool		g_bBloom = false;
bool		g_bOutline = false;
bool		g_bGlow = false;

float		g_AccTime = 0.f;

vector		g_vOutlineColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vGlowColor = vector(1.f, 1.f, 1.f, 1.f);


texture2D	g_DecalTargetTexture;
texture2D	g_DecalDepthTexture;
texture2D	g_DepthTexture;

texture2D	g_NoiseTexture;
texture2D	g_DissolveTexture;
texture2D	g_DissolveNoiseTexture;
texture2D	g_DissolveColorTexture;

texture2D	g_MaskTexture;
float		g_fDistortionStrength = 0.f;
float2		g_vUVSpeed_Mask;

bool		g_bIsDissolve = false;
float4		g_vDissolveColor = { 0.f, 0.f, 0.f, 0.f };
float		g_fDissolveStrength = 0.f;
float		g_fTimeAccDissolve = 0.f;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};
struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
};


struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
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
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{	
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vGlow: SV_TARGET3;
	//vector		vBloom : SV_TARGET4;
	vector		vOutlineFlag : SV_TARGET5;
};


vector Get_Bloom(vector vDiffuseColor)
{
	float	fAver = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	
	if (fAver > 0.3f)
	{
		return vDiffuseColor * g_fBloomPower;
	}
	else
		return vector(0.f, 0.f, 0.f, 0.f);
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	float fAtt = g_fSaturationRate; // 채도 비율

	vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
	vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
	vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;

	Out.vDiffuse = vDiffuseColor;
    
	if (g_bIsDissolve)
	{
		vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
		vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

		if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
        {
            Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
            //Out.vDiffuse.a = 0.1f;
            Out.vGlow.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			Out.vGlow.a = 1.f;

        }

        if (g_fTimeAccDissolve - g_fDissolveStrength > vDissolveNoiseTexture.r)
			discard;
	}

	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    if (g_bOutline)
    {
        Out.vOutlineFlag = g_fOutlineColor * g_fLineBrightness;
        Out.vOutlineFlag.a = 1.f;
    }
    
	//Out.vGlow = vector(0.f, 0.f, 0.f, 0.f);

	//if(true == g_bBloom)
	//	Out.vBloom += Get_Bloom(Out.vDiffuse);

	//if (true == g_bGlow)
	//	Out.vGlow += g_vGlowColor;

	//if (true == g_bOutline)
	//	Out.vOutline = g_vOutlineColor;

	return Out;
}

PS_OUT PS_MAIN_GRASS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
	float			fDistortionPower = 0.f;
	float2			vTexUV_Mask;

	vTexUV_Mask.x = In.vTexUV.x + g_vUVSpeed_Mask.x;
	vTexUV_Mask.y = In.vTexUV.y + g_vUVSpeed_Mask.y;

	vector			vDistortion = g_MaskTexture.Sample(DefaultSampler, vTexUV_Mask);

	if (0.f != vDistortion.g)
		fDistortionPower = g_fDistortionStrength / 1000.f;
	/*    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
	*/

	vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV + vDistortion.r * fDistortionPower) * g_fDiffusePower/* * vNoise*/;

	float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	float fAtt = g_fSaturationRate; // 채도 비율

	vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
	vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
	vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;

	Out.vDiffuse = vDiffuseColor;

	if (g_bIsDissolve)
	{
		vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
		vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

		if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
		{
			Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			//Out.vDiffuse.a = 0.1f;
			Out.vGlow.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			Out.vGlow.a = 1.f;

		}

		if (g_fTimeAccDissolve - g_fDissolveStrength > vDissolveNoiseTexture.r)
			discard;
	}

	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	//Out.vGlow = vector(0.f, 0.f, 0.f, 0.f);

	//if (true == g_bBloom)
	//	Out.vBloom += Get_Bloom(Out.vDiffuse);

	return Out;
}

PS_OUT PS_DOORLIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float		fShade, fSpecular;
	fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	vector		vLook = In.vWorldPos - g_vCamPosition;
	vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vDiffuse = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;


	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse += g_fBrightness;

	//Out.vBloom = vector(1.f, 1.f, 1.f, 1.f);
	//Out.vBloom = Get_Bloom(Out.vDiffuse);

	//Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

	return Out;
}

struct PS_OUT_DEPTH
{
	//vector		vDiffuse : SV_TARGET0;
	//vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET0;
};

PS_OUT_DEPTH PS_DEPTH(PS_IN In)
{
	PS_OUT_DEPTH		Out = (PS_OUT_DEPTH)0;

	//vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	//if (vDiffuse.a < 0.1f)
	//	discard;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    ////(x - 0.9f) * 10.f
    //Out.vDepth.r = (In.vProjPos.z / In.vProjPos.w - 0.9f) * 10.f;
    //Out.vDepth.g = (In.vProjPos.w / g_fFar - 0.9f) * 10.f;
    //Out.vDepth.b = 0.f;
    //Out.vDepth.a = 1.f;

	return Out;
}


struct PS_OUT_DECAL_TEXTURE
{
	//vector		vDiffuse : SV_TARGET0;
	//vector		vNormal : SV_TARGET1;
	vector		vTexture : SV_TARGET0;
};

PS_OUT_DECAL_TEXTURE PS_DECAL_TEXTURE(PS_IN In)
{
	PS_OUT_DECAL_TEXTURE		Out = (PS_OUT_DECAL_TEXTURE)0;

	Out.vTexture = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower;

	if (Out.vTexture.a < 0.1f)
		discard;
	//Out.vTexture.a = 1.f;

	return Out;
}





struct VS_OUT_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
	VS_OUT_NORMAL		Out = (VS_OUT_NORMAL)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}



struct PS_IN_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;

};


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;
	vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	// 채도 연산
	float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	float fAtt = g_fSaturationRate; // 채도 비율

	vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
	vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
	vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;

	Out.vDiffuse = vDiffuseColor;

	/* 0 ~ 1 */

	// 노말맵 반복시켜주기 위한 코드
	float2		vTexUV;
	vTexUV.x = In.vTexUV.x * g_CX;
	vTexUV.y = In.vTexUV.y * g_CZ;


	float3		vPixelNormal = g_NormalTexture.Sample(DefaultSampler, vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f - 0.5f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}



PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	float fAtt = g_fSaturationRate; // 채도 비율

	vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
	vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
	vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;


	vector		vDissolve = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV * g_fUVRange);
	if (g_AccTime > vDissolve.r)
		vDiffuseColor.a = 0.f;

	Out.vDiffuse = vDiffuseColor;
	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	//if (true == g_bBloom)
	//	Out.vBloom += Get_Bloom(Out.vDiffuse);

	return Out;
}


PS_OUT PS_MAIN_DISSOLVE_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;
	vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

	// 채도 연산
	float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
	float fAtt = g_fSaturationRate; // 채도 비율

	vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
	vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
	vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;

	vector		vDissolve = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV * g_fUVRange);
	if (g_AccTime > vDissolve.r)
		vDiffuseColor.a = 0.f;

	Out.vDiffuse = vDiffuseColor;

	/* 0 ~ 1 */

	// 노말맵 반복시켜주기 위한 코드
	float2		vTexUV;
	vTexUV.x = In.vTexUV.x * g_CX;
	vTexUV.y = In.vTexUV.y * g_CZ;


	float3		vPixelNormal = g_NormalTexture.Sample(DefaultSampler, vTexUV).xyz;

	/* -1 ~ 1 */
	vPixelNormal = vPixelNormal * 2.f - 1.f - 0.5f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_CLOUD(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fDistortionPower = 0.f;
    float2 vTexUV_Mask;


    //vector vDistortion = g_MaskTexture.Sample(DefaultSampler, vTexUV_Mask);

    //if (0.f != vDistortion.g)
    //    fDistortionPower = g_fDistortionStrength / 1000.f;

    vector vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV /*+ vDistortion.r * fDistortionPower*/);

    float fAverColor = (vDiffuseColor.r + vDiffuseColor.g + vDiffuseColor.b) / 3.f;
    float fAtt = g_fSaturationRate; // 채도 비율

    vDiffuseColor.r = fAverColor + (vDiffuseColor.r - fAverColor) * fAtt;
    vDiffuseColor.g = fAverColor + (vDiffuseColor.g - fAverColor) * fAtt;
    vDiffuseColor.b = fAverColor + (vDiffuseColor.b - fAverColor) * fAtt;

    Out.vDiffuse = vDiffuseColor;

    if (g_bIsDissolve)
    {
        vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
        vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

        if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
        {
            Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			//Out.vDiffuse.a = 0.1f;
            Out.vGlow.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
            Out.vGlow.a = 1.f;

        }

        if (g_fTimeAccDissolve - g_fDissolveStrength > vDissolveNoiseTexture.r)
            discard;
    }

	//Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_fDiffusePower/* * vNoise*/;

    if (Out.vDiffuse.a < 0.1f)
        discard;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

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
	pass DoorLight
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DOORLIGHT();
	}
	pass Depth
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DEPTH();
	}
	pass DecalTexture
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DECAL_TEXTURE();
	}
	pass NormalMapping
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass Dissolve_Default //5
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
	}

	pass Dissolve_Normal
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE_NORMAL();
	}
	pass CullModeNone	//7
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Grass
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GRASS();
	}
}