#include "LightDef.hlsl"
#include "SimpleLighting.hlsl"

Texture2D txDiffuse : register( t0 );
Texture2D txNormal : register(t1);

SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
	DirectionalLight DirLight;
	PointLight PLight;
};

cbuffer cbChangeOnResize : register( b1 )
{
};

cbuffer cbPerFrame : register( b2 )
{
	float3 eyePos;
	float pad;
};

cbuffer cbPerObject: register( b3 )
{
    float4x4 matWorld;
    float4x4 matWorldInvTranspose;
    float4x4 matWVP;
	Material material;	
};

struct VS_INPUT
{
    float3 PosL : POSITION;
    float3 NorL : NORMAL;
    float2 Tex : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NorW : NORMAL ;
	float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
};


float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	float3 normalT = 2.0f*normalMapSample - 1.0f;
	
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpedNormalW = normalize(mul(normalT, TBN));
	
	return bumpedNormalW;
}

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

	output.PosW = mul(float4( input.PosL, 1.0f ), matWorld).xyz;
	output.NorW = mul(input.NorL, (float3x3)matWorldInvTranspose);
	output.TangentW = mul(input.TangentL, (float3x3)matWorld);
	output.PosH = mul(float4(input.PosL, 1.0f), matWVP);
	output.Tex = input.Tex; 

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
	input.NorW = normalize(input.NorW);
	float3 toEye = eyePos - input.PosW;
	float distToEye = length(toEye);
	toEye /= distToEye;
	
	float4 texColor = txDiffuse.Sample(samLinear, input.Tex);

	float3 normalMapSample = txNormal.Sample(samLinear, input.Tex).rgb;
	float3 bumpedNormal = NormalSampleToWorldSpace(normalMapSample, input.NorW, input.TangentW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;
	ComputeDirectionalLight(material, DirLight, bumpedNormal, toEye, A, D, S);

	ambient += A;
	diffuse += D;
	specular += S;

	ComputePointLight(material, PLight, input.PosW, bumpedNormal, toEye, A, D, S);

	ambient += A;
	diffuse += D;
	specular += S;

	float4 litColor = texColor * (ambient + diffuse) + specular;
	litColor.a = texColor.a;
	
	return litColor;
}

