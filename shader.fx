// global vars
// World*View*Proj mátrix
uniform extern float4x4 WorldViewProjection;
uniform extern float4 MaterialColor;

float Transparency = 0.75;

// textúra
uniform extern texture Texture_A;
sampler Sampler = sampler_state
{
	Texture = <Texture_A>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU = WRAP;
	AddressV = WRAP;
};

// vertexárnyaló eredményét tároló struktúra
struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD0;
};

// vertexárnyaló
VS_OUTPUT vs_main(float3 pos_in : POSITION0, float2 tex_in : TEXCOORD0)
{
	// eredmény struktúra kinullázása
	VS_OUTPUT outVS = (VS_OUTPUT)0;

	// transzformáció végrehajtása
	outVS.Position = mul(float4(pos_in, 1.0f), WorldViewProjection);
	outVS.Color.rgb = MaterialColor;
	outVS.Color.a = Transparency;
	outVS.Tex = tex_in;

	return outVS;
}

// képpont árnyaló
float4 ps_main(float4 color_in : COLOR0, float2 tex_in : TEXCOORD0) : COLOR
{
	float3 texColor = tex2D(Sampler, tex_in).rgb;

	return float4(color_in * texColor, color_in.a);
}

// technika
technique Tech
{
	// lépés
	pass P0
	{
		// állapotok
		//FillMode = Wireframe;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		BlendOp = ADD;

		// a P0 lépés csúcspont- és képpontárnyalója
		vertexShader = compile vs_2_0 vs_main(); // VS 2.0
		pixelShader = compile ps_2_0 ps_main(); // PS 2.0
	}
}