
string Model : ModelData = "Sphere.x";


struct VS_INPUT{
	float4 mPosition : POSITION;
	//float4 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT{
	float4 mPosition : POSITION;
	//float4 mTexCoord : TEXCOORD0;
};
float4x4 gWorld : World;
float4x4 gView : View;
float4x4 gProj : Projection;

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorld);
	Output.mPosition = mul(Input.mPosition, gView);
	Output.mPosition = mul(Input.mPosition, gProj);

	//Output.mTexCoord = Input.mTexCoord;

	return Output;
}

texture DiffuseMap < string ResourceName = "wood.jpg"; > ;
sampler2D DiffuseSampler = sampler_state{
	Texture = (DiffuseMap);
};

struct PS_INPUT{
	float2 mTexCoord : TEXCOORD0;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
	//float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	//return albedo.rgba;
	return( float4( 1.0f, 0.0f, 0.0f, 1.0f ) );
}

technique SphereVS
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 main();
		PixelShader = compile ps_3_0 ps_main();
	}

}