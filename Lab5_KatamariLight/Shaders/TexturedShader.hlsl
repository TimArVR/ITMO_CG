#pragma pack_matrix(row_major) //BeLight

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 tex : TEXCOORD;
    float4 normal : NORMAL; //BeLight
    float4 worldPos : WORLDPOS; //BeLight
};

struct VS_IN
{
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL0; //BeLight
};

struct ConstData
{
    matrix WorldView;
};

cbuffer ConstBuf : register(b0)
{
    //ConstData constData; //BeLight
    float4x4 worldViewProjMatrix; //BeLight
    float4x4 invertedMatrix; //BeLight
    float4x4 worldMatrix; //BeLight
};

cbuffer SceneConstBuf : register(b1) //BeLight
{
    float4 lightDirection;
    float4 lightColor;
    float4 viewDirectionSpecular;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

float4 PSMain(PS_IN input) : SV_Target
{
    float4 ambient = lightColor.w * float4(lightColor.xyz, 1.0f); //BeLight
    
    float4 tex = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);

    float4 norm = normalize(input.normal); //BeLight
    float diff = max(dot(norm, lightDirection), 0.0f); //BeLight
    float4 diffuse = diff * float4(lightColor.xyz, 1.0f); //BeLight

    float4 reflectDir = reflect(-lightDirection, norm); //BeLight
    float3 viewdir = normalize(viewDirectionSpecular.xyz - input.worldPos.xyz); //BeLight
    float spec = pow(max(dot(viewdir, reflectDir.xyz), 0.0f), 16); //BeLight
    float4 specular = viewDirectionSpecular.w * spec /* * float4(viewDirectionSpecular.xyz, 1.0f)*/; //BeLight

    float4 result = (ambient + diffuse + specular) * tex; //BeLight

    return float4(result.xyz, 1.0f); //BeLight
    
    //return tex;//BeLight
}

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    //output.pos = mul(input.pos, constData.WorldView);//BeLight
    output.pos = mul(float4(input.pos.xyz, 1.0f), worldViewProjMatrix); //BeLight
    output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), invertedMatrix); //BeLight
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), worldMatrix); //BeLight
    return output;
}