
sampler PointSampler = sampler_state
{
	filter = min_mag_mip_Point;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointClampSampler = sampler_state
{
	filter = min_mag_mip_Point;
	AddressU = clamp;
	AddressV = clamp;
};

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

BlendState BS_AlphaBlendingDefault
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = dest_alpha;
	BlendOp = max;
};

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
};


BlendState BS_BlendAlphaMax
{	
	BlendEnable[0] = true;
	//BlendEnable[1] = true;
	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
	BlendOpAlpha = max;
};

BlendState BS_BlendMax
{	
	BlendEnable[0] = true;
	//BlendEnable[1] = true;
	SrcBlend = src_alpha;
	DestBlend = dest_alpha;
	BlendOp = max;
	BlendOpAlpha = max;
};


BlendState BS_BlendAlphaMin
{
	BlendEnable[0] = true;
	//BlendEnable[1] = true;
	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
	BlendOpAlpha = min;
};


BlendState BS_BlendAlphaControl
{
	BlendEnable[0] = true;
	//BlendEnable[1] = true;
	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
	SrcBlendAlpha = zero;
	DestBlendAlpha = One;
	BlendOpAlpha = Add;
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

DepthStencilState DSS_ZEnable_Zero
{
	DepthEnable = true;
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

RasterizerState RS_CullModeNone
{
	FillMode = solid;
	CullMode = none;
	FrontCounterClockwise = false;
};