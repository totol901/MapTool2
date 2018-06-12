#pragma once
class TerrainShaderClass
{
public:
	TerrainShaderClass();
	~TerrainShaderClass();

private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

	struct TextureInfoBufferType
	{
		bool useAlplha;
		D3DXVECTOR3 padding2;
	};

public:
	bool Initialize(ID3D11Device* device, HWND hwnd);
	bool BlendInitialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext * deviceContext,
		int indexCount, D3DXMATRIX worldMat, D3DXMATRIX viewMat, D3DXMATRIX projMat,
		D3DXVECTOR4 ambientolor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection
	, ID3D11ShaderResourceView* texture);
	
	//컬러 셰이더
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection
		, ID3D11ShaderResourceView* texture);
	bool SetShaderTextures(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, bool);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	//블랜드
	bool SetBlendShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	bool InitializeBlendShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_textureInfoBuffer;
};

