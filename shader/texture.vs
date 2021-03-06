////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// ?μ ???λ ¬ κ³μ°???ν΄ ?μΉ λ²‘ν°λ₯?4 ?¨μλ‘?λ³κ²½ν©?λ€.
	input.position.w = 1.0f;

	// ?λ, λ·?λ°??¬μ ?λ ¬??????μ ???μΉλ₯??βκ³?°ν©?λ€.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// ?½μ? ?μ΄?μ ?μ€μ²?μ’νλ₯???₯ν??
	output.tex = input.tex;
    
    return output;
}