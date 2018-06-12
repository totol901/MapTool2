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
    

	// ?ì ˆ???‰ë ¬ ê³„ì‚°???„í•´ ?„ì¹˜ ë²¡í„°ë¥?4 ?¨ìœ„ë¡?ë³€ê²½í•©?ˆë‹¤.
	input.position.w = 1.0f;

	// ?”ë“œ, ë·?ë°??¬ì˜ ?‰ë ¬???€???•ì ???„ì¹˜ë¥??‹â€‹ê³„?°í•©?ˆë‹¤.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// ?½ì? ?ì´?”ì˜ ?ìŠ¤ì²?ì¢Œí‘œë¥??€?¥í•œ??
	output.tex = input.tex;
    
    return output;
}