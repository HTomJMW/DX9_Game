// global vars
// World*View*Proj m�trix
uniform extern float4x4 WorldViewProjection;
uniform extern float4 MaterialColor;

float Transparency = 0.75;

// text�ra
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

// vertex�rnyal� eredm�ny�t t�rol� strukt�ra
struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD0;
};

// vertex�rnyal�
VS_OUTPUT vs_main(float3 pos_in : POSITION0, float2 tex_in : TEXCOORD0)
{
	// eredm�ny strukt�ra kinull�z�sa
	VS_OUTPUT outVS = (VS_OUTPUT)0;

	// transzform�ci� v�grehajt�sa
	outVS.Position = mul(float4(pos_in, 1.0f), WorldViewProjection);
	outVS.Color.rgb = MaterialColor;
	outVS.Color.a = Transparency;
	outVS.Tex = tex_in;

	return outVS;
}

// k�ppont �rnyal�
float4 ps_main(float4 color_in : COLOR0, float2 tex_in : TEXCOORD0) : COLOR
{
	float3 texColor = tex2D(Sampler, tex_in).rgb;

	return float4(color_in * texColor, color_in.a);
}

// technika
technique Tech
{
	// l�p�s
	pass P0
	{
		// �llapotok
		//FillMode = Wireframe;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		BlendOp = ADD;

		// a P0 l�p�s cs�cspont- �s k�ppont�rnyal�ja
		vertexShader = compile vs_2_0 vs_main(); // VS 2.0
		pixelShader = compile ps_2_0 ps_main(); // PS 2.0
	}
}