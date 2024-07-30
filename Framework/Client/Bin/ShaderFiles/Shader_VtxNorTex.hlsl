
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float	g_fFar;

//float   g_fScale;

vector	g_vBrushPos = vector(15.f, 0.f, 15.f, 1.f);
float	g_fBrushRange = 10.f;

float2	g_vOffset;

// vector	g_vMtrlDiffuse;
//texture2D	g_SourDiffTexture;
//texture2D	g_DestDiffTexture;
texture2D	g_FilterTexture;
texture2D   g_FilterTexture_2;

//texture2D	g_BrushTexture;


texture2D	g_RDiffTexture;
texture2D	g_GDiffTexture;
texture2D	g_BDiffTexture;
texture2D	g_BaseDiffTexture;

texture2D   g_RDiffTexture_2;
texture2D   g_GDiffTexture_2;

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
};

struct VS_OUT_PHONG
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT_PHONG VS_MAIN_TERRAIN_PHONG(VS_IN In)
{
	VS_OUT_PHONG		Out = (VS_OUT_PHONG)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct VS_OUT_TESS
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT_TESS VS_MAIN_TERRAIN_PHONG_TESS(VS_IN In)
{
    VS_OUT_TESS Out = (VS_OUT_TESS) 0;

    matrix matWV, matWVP;

    Out.vPosition = In.vPosition;
    Out.vNormal = In.vNormal;
    Out.vTexUV = In.vTexUV;

    return Out;
}


struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;

	// Geometry cubic generated control points
    float3 f3B210 : POSITION3;
    float3 f3B120 : POSITION4;
    float3 f3B021 : POSITION5;
    float3 f3B012 : POSITION6;
    float3 f3B102 : POSITION7;
    float3 f3B201 : POSITION8;
    float3 f3B111 : CENTER;

	// Normal quadratic generated control points
    float3 f3N110 : NORMAL3;
    float3 f3N011 : NORMAL4;
    float3 f3N101 : NORMAL5;
};

PatchTess ConstantHS(InputPatch<VS_OUT_TESS, 3> Patch, uint PatchID : SV_PrimitiveID)
{
    PatchTess pt;

    pt.EdgeTess[0] = 4;
    pt.EdgeTess[1] = 4;
    pt.EdgeTess[2] = 4;

    pt.InsideTess = (pt.EdgeTess[0] + pt.EdgeTess[1] + pt.EdgeTess[2]) / 3.f;

	// Assign Positions
    float3 f3B003 = Patch[0].vPosition;
    float3 f3B030 = Patch[1].vPosition;
    float3 f3B300 = Patch[2].vPosition;
	// And Normals
    float3 f3N002 = Patch[0].vNormal;
    float3 f3N020 = Patch[1].vNormal;
    float3 f3N200 = Patch[2].vNormal;

	// Compute the cubic geometry control points
	// Edge control points
    pt.f3B210 = ((2.0f * f3B003) + f3B030 - (dot((f3B030 - f3B003), f3N002) * f3N002)) / 3.0f;
    pt.f3B120 = ((2.0f * f3B030) + f3B003 - (dot((f3B003 - f3B030), f3N020) * f3N020)) / 3.0f;
    pt.f3B021 = ((2.0f * f3B030) + f3B300 - (dot((f3B300 - f3B030), f3N020) * f3N020)) / 3.0f;
    pt.f3B012 = ((2.0f * f3B300) + f3B030 - (dot((f3B030 - f3B300), f3N200) * f3N200)) / 3.0f;
    pt.f3B102 = ((2.0f * f3B300) + f3B003 - (dot((f3B003 - f3B300), f3N200) * f3N200)) / 3.0f;
    pt.f3B201 = ((2.0f * f3B003) + f3B300 - (dot((f3B300 - f3B003), f3N002) * f3N002)) / 3.0f;

	// Center control point
    float3 f3E = (pt.f3B210 + pt.f3B120 + pt.f3B021 + pt.f3B012 + pt.f3B102 + pt.f3B201) / 6.0f;
    float3 f3V = (f3B003 + f3B030 + f3B300) / 3.0f;
    pt.f3B111 = f3E + ((f3E - f3V) / 2.0f);

	// Compute the quadratic normal control points, and rotate into world space
    float fV12 = 2.0f * dot(f3B030 - f3B003, f3N002 + f3N020) / dot(f3B030 - f3B003, f3B030 - f3B003);
    pt.f3N110 = normalize(f3N002 + f3N020 - fV12 * (f3B030 - f3B003));
    float fV23 = 2.0f * dot(f3B300 - f3B030, f3N020 + f3N200) / dot(f3B300 - f3B030, f3B300 - f3B030);
    pt.f3N011 = normalize(f3N020 + f3N200 - fV23 * (f3B300 - f3B030));
    float fV31 = 2.0f * dot(f3B003 - f3B300, f3N200 + f3N002) / dot(f3B003 - f3B300, f3B003 - f3B300);
    pt.f3N101 = normalize(f3N200 + f3N002 - fV31 * (f3B003 - f3B300));

    return pt;
}

struct HullOut
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
};


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(9.0f)]

HullOut HS_MAIN(InputPatch<VS_OUT_TESS, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID)
{
    HullOut hout;

    hout.vPosition = p[i].vPosition;
    hout.vNormal = p[i].vNormal;
    hout.vTexUV = p[i].vTexUV;

    return hout;
}

struct DomainOut
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

[domain("tri")]
DomainOut DS_MAIN(PatchTess PatchTess, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
	// The barycentric coordinates
    float fU = uvw.x;
    float fV = uvw.y;
    float fW = uvw.z;

	// Precompute squares and squares * 3 
    float fUU = fU * fU;
    float fVV = fV * fV;
    float fWW = fW * fW;
    float fUU3 = fUU * 3.0f;
    float fVV3 = fVV * 3.0f;
    float fWW3 = fWW * 3.0f;

    DomainOut Out = (DomainOut) 0;
	
    float3 p = tri[0].vPosition * fWW * fW +
		tri[1].vPosition * fUU * fU +
		tri[2].vPosition * fVV * fV +
		PatchTess.f3B210 * fWW3 * fU +
		PatchTess.f3B120 * fW * fUU3 +
		PatchTess.f3B201 * fWW3 * fV +
		PatchTess.f3B021 * fUU3 * fV +
		PatchTess.f3B102 * fW * fVV3 +
		PatchTess.f3B012 * fU * fVV3 +
		PatchTess.f3B111 * 6.0f * fW * fU * fV;

	// Compute normal from quadratic control points and barycentric coords
    float3 n = tri[0].vNormal * fWW +
		tri[1].vNormal * fUU +
		tri[2].vNormal * fVV +
		PatchTess.f3N110 * fW * fU +
		PatchTess.f3N011 * fU * fV +
		PatchTess.f3N101 * fW * fV;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(p, 1.f), matWVP);
    Out.vTexUV = tri[0].vTexUV * fW + tri[1].vTexUV * fU + tri[2].vTexUV * fV;
    Out.vNormal = normalize(mul(vector(n, 0.f), g_WorldMatrix));
    Out.vWorldPos = mul(vector(p, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;

    return Out;
}



struct PS_IN_PHONG
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
    //vector      vToonFlag : SV_TARGET3;
    vector      vGlow : SV_TARGET3;
    vector      vTerrainFlag : SV_TARGET4;
    vector      vOutlineFlag : SV_TARGET5;
    vector      vToonFlag : SV_TARGET6;
};

PS_OUT PS_MAIN_TERRAIN_PHONG(PS_IN_PHONG In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector		vSourDiffuse = g_SourDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);
	//vector		vDestDiffuse = g_DestDiffTexture.Sample(DefaultSampler, In.vTexUV * 30.f);
	vector		vRDiffuse = g_RDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	vector		vGDiffuse = g_GDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	vector		vBDiffuse = g_BDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	vector		vBaseDiffuse = g_BaseDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);

	vector		vRDiffuse_2 = g_RDiffTexture_2.Sample(DefaultSampler, In.vTexUV * 80.f);
	vector		vGDiffuse_2 = g_GDiffTexture_2.Sample(DefaultSampler, In.vTexUV * 200.f);
    
    
	//float2		vOffset = {-0.05f, -0.05f};
	float2		vFinalUV = In.vTexUV;
	vFinalUV.y = 1.f - vFinalUV.y;

	vector		vFilter = g_FilterTexture.Sample(PointSampler, vFinalUV);
	vector		vFilter_2 = g_FilterTexture_2.Sample(PointSampler, vFinalUV);

    
	//vector		vBrush = (vector)0;

	//if (g_vBrushPos.x - g_fBrushRange * 0.5f < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange * 0.5f &&
	//	g_vBrushPos.z - g_fBrushRange * 0.5f < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange * 0.5f)
	//{
	//	float2	vTexUV;

	//	vTexUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange * 0.5f)) / g_fBrushRange;
	//	vTexUV.y = ((g_vBrushPos.z + g_fBrushRange * 0.5f) - In.vWorldPos.z) / g_fBrushRange;

	//	vBrush = g_BrushTexture.Sample(DefaultSampler, vTexUV);
	//}

	//Out.vDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter) + vBrush;

	//Out.vDiffuse = vBaseDiffuse;

	//float	fAllColor = length(vFilter.rgb);
    
	//Out.vDiffuse = (1.f - fAllColor) * vBaseDiffuse +
    //vFilter.r * vRDiffuse +
	    //	vFilter.g * vGDiffuse +
	    //	vFilter.b * vBDiffuse;
    //float fBaseColorRate = saturate(1.f - fTatalColor);
    
    
    Out.vDiffuse = (1.f - vFilter.r) * vBaseDiffuse + vFilter.r * vRDiffuse;
    Out.vDiffuse = (1.f - vFilter.g) * Out.vDiffuse + vFilter.g * vGDiffuse;
    Out.vDiffuse = (1.f - vFilter.b) * Out.vDiffuse + vFilter.b * vBDiffuse;

    Out.vDiffuse = (1.f - vFilter_2.r) * Out.vDiffuse + vFilter_2.r * vRDiffuse_2;
    Out.vDiffuse = (1.f - vFilter_2.g) * Out.vDiffuse + vFilter_2.g * vGDiffuse_2;

    
    

	Out.vDiffuse.a = 1.f;


    //Out.vToonFlag.r = 1.f;

	/* -1 -> 0, 1 -> 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);



	/* x : 0 ~ 1 */
	/* y : n ~ f 정규화하여 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	Out.vGlow = vector(0.f, 0.f, 0.f, 0.f);
    Out.vTerrainFlag.r = 1.f;
    
    Out.vOutlineFlag = 0.f;
    Out.vToonFlag = 0.f;
    
	return Out;
}

struct PS_OUT_DEPTH
{
    vector vDepth : SV_TARGET0;
};

PS_OUT_DEPTH PS_DEPTH(PS_IN_PHONG In)
{
    PS_OUT_DEPTH Out = (PS_OUT_DEPTH) 0;

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    ////(x - 0.9f) * 10.f
    //Out.vDepth.r = (In.vProjPos.z / In.vProjPos.w - 0.9f) * 10.f;
    //Out.vDepth.g = (In.vProjPos.w / g_fFar - 0.9f) * 10.f;
    //Out.vDepth.b = 0.f;
    //Out.vDepth.a = 1.f;
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Terrain_Phong
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		//SetRasterizerState(RS_Wireframe);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_PHONG();
	}

    pass Depth
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
		//SetRasterizerState(RS_Wireframe);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEPTH();
    }

    pass Terrain_Phong_Tess
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
		//SetRasterizerState(RS_Wireframe);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG_TESS();
        GeometryShader = NULL;
        HullShader = compile hs_5_0 HS_MAIN();
        DomainShader = compile ds_5_0 DS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_PHONG();
    }

    pass Terrain_Phong_Tess_Depth
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
		//SetRasterizerState(RS_Wireframe);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG_TESS();
        GeometryShader = NULL;
        HullShader = compile hs_5_0 HS_MAIN();
        DomainShader = compile ds_5_0 DS_MAIN();
        PixelShader = compile ps_5_0 PS_DEPTH();
    }

}