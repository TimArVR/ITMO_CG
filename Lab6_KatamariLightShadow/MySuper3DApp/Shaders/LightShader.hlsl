struct PS_IN {
    float4 pos : SV_POSITION;
    float4 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 objectPos : POSITION;
};

struct VS_IN {
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL0;
};

cbuffer ConstBuffer : register(b0) {
    float4x4 viewProjectionMatrix;
    float4x4 object;
    float4x4 invertedTransposedMatrix;
    float4x4 lightSpace;//
};

cbuffer SecondConstBuffer : register(b1) {
    float4 lightDir;
    float4 lightColorAmbStr;
    float4 viewDirSpecStr;
};

Texture2D DiffuseMap : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input) {
    PS_IN output = (PS_IN) 0;

    output.pos = mul(viewProjectionMatrix, mul(object, input.pos));
    output.tex = input.tex;
    output.objectPos = mul(object, input.pos).xyz;
    output.normal = mul(invertedTransposedMatrix, input.normal).xyz;
    
    return output;
}

float4 PSMain(PS_IN input) : SV_Target {
	float3 norm = normalize(input.normal);
	float4 ambient = lightColorAmbStr.w * float4(lightColorAmbStr.xyz, 1.0f);
    
	float4 tex = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);

	float diff = max(dot(norm, -lightDir.xyz), 0.0f);
	float4 diffuse = diff * float4(lightColorAmbStr.xyz, 1.0f);
    
	float3 cameraPos = viewDirSpecStr.xyz;
    float3 viewDirection = normalize(cameraPos - input.objectPos);
    
	float3 reflectDir = reflect(lightDir.xyz, norm);
	float spec = pow(max(dot(viewDirection, reflectDir.xyz), 0.0f), 32);
	float4 specular = 0.5f * spec;

    
    float4 lightSpacePos = mul(lightSpace, float4(input.objectPos, 1.0f));
    float shadowDepth = ShadowMap.SampleLevel(Sampler, float2(lightSpacePos.x, -lightSpacePos.y) * 0.5f + 0.5f, 0).x;
    float shadowCoef = lightSpacePos.z - 0.0005f < shadowDepth;

    
    float4 result = (ambient + (diffuse + specular) * shadowCoef) * tex;
    
	return float4(result.xyz, 1.0f);
}

