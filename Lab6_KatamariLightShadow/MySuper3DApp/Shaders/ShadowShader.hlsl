struct PS_IN {
    float4 pos : SV_POSITION;
};

struct VS_IN {
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL0;
};

cbuffer ConstBuffer : register(b0) {
    float4x4 viewProjectionMatrix;
    float4x4 model;
};

PS_IN VSMain(VS_IN input) {
    PS_IN output = (PS_IN)0;
    
    output.pos = mul(viewProjectionMatrix, mul(model, input.pos));
    
    return output;
}
