
#define _WATER
#define _SSR				// Screen Space Reflaction, Used Water
#define _BLUR
#define _HDR				// High Quality Render 
#define _LENSFLARE
#define _SSAO				// Screen Space Ambient Occlusion


sampler PointSampler = sampler_state
{
	filter = min_mag_mip_Point;
	AddressU = wrap;
	AddressV = wrap;
};

// default
sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_Linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler LinearClampSampler = sampler_state
{
	filter = min_mag_mip_Linear;
	AddressU = clamp;
	AddressV = clamp;
};



BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AllColorBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	
	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};


BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
};








//BOOL DepthEnable;
//D3D11_DEPTH_WRITE_MASK DepthWriteMask;
//D3D11_COMPARISON_FUNC DepthFunc;
//BOOL StencilEnable;
//UINT8 StencilReadMask;
//UINT8 StencilWriteMask;
//D3D11_DEPTH_STENCILOP_DESC FrontFace;
//D3D11_DEPTH_STENCILOP_DESC BackFace;

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};


/*
D3D11_FILL_MODE FillMode;
D3D11_CULL_MODE CullMode;
BOOL FrontCounterClockwise;
INT DepthBias;
FLOAT DepthBiasClamp;
FLOAT SlopeScaledDepthBias;
BOOL DepthClipEnable;
BOOL ScissorEnable;
BOOL MultisampleEnable;
BOOL AntialiasedLineEnable;
*/

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
CullMode = back;
FrontCounterClockwise = false;
};

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Sky
{
	FillMode = solid;
	CullMode = front;
	FrontCounterClockwise = false;
};
