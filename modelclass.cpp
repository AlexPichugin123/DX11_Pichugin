////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}
bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}
void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//////////////////////////////////////////////////////////////////////////////////
	int i, j; // i и j количество вертексов и индексов для квадрата из 2 треугольников
	float yOffset(0);
	int n(24), countSquare(0), columns(6), counts(0);//допустим хочу N квадратов сделать, и columns столбцов
	//////////////////////////////////////////////////////////////////////////////////

	// Set the number of vertices in the vertex array.
	m_vertexCount = n * 4;

	// Set the number of indices in the index array.
	m_indexCount = n * 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// сделать от 0 до 5, 6 поинтов, по ним понятно в какой позиции должна быть вершина для квадрата из 2 треугольников,тип делишь 6 на Iтое и понимаешь, 
	// потом сделать снова деление на 6 чтобы понимать какой по счету квадрат, и дальше понимать где его разместить

	//auto verticesBuf = &vertices;

	for (
		i = 0, j = 0;
		i < m_vertexCount;
		i += 4, j += 6
		)
	{
		vertices[i].position = XMFLOAT3(countSquare * 4, 0.0f + yOffset, 0.0f);
		vertices[i].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[i + 1].position = XMFLOAT3(countSquare * 4, 4.0f + yOffset, 0.0f);
		vertices[i + 1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[i + 2].position = XMFLOAT3(countSquare * 4 + 4, 4.0f + yOffset, 0.0f);
		vertices[i + 2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[i + 3].position = XMFLOAT3(countSquare * 4 + 4, 0.0f + yOffset, 0.0f);
		vertices[i + 3].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		/*verticesBuf[i+0] = &vertices[i + 1];
		verticesBuf[i+1] = &vertices[i + 2];
		verticesBuf[i+2] = &vertices[i];
		verticesBuf[i+3] = &vertices[i + 3];*/


		indices[j] = i;
		indices[j + 1] = i + 1;
		indices[j + 2] = i + 2;

		indices[j + 3] = i;
		indices[j + 4] = i + 2;
		indices[j + 5] = i + 3;

		countSquare++; //счетчик квадратов, так-же использую для X координат выше
		//counts++;

		if (countSquare % columns == 0) //%columns сколько квадратов в строке
		{
			yOffset -= 4; //смещение по координатам вниз для новых строк
			countSquare = 0;
		}
	}
	float angle = (360.f / (float)columns) * 3.141519 / 180.f;
	double pi = atan(1) * 4;
	float Radius = 5; //fabs(vertices[columns * 4 - 1].position.x - vertices[0].position.x) / (2 * pi);//////////////////////////
	int i1(0), j1(1);

	for (i1; i1 < m_vertexCount; i1 += 4)
	{
		vertices[i1 + 0].position.x = cos(angle * j1) * Radius;
		vertices[i1 + 0].position.z = sin(angle * j1) * Radius;

		vertices[i1 + 1].position.x = cos(angle * j1) * Radius;
		vertices[i1 + 1].position.z = sin(angle * j1) * Radius;

		vertices[i1 + 2].position.x = cos(angle * j1 + 1) * Radius;
		vertices[i1 + 2].position.z = sin(angle * j1 + 1) * Radius;

		vertices[i1 + 3].position.x = cos(angle * j1 + 1) * Radius;
		vertices[i1 + 3].position.z = sin(angle * j1 + 1) * Radius;

		j1++;
		if (j1 == columns+1)
			{
				j1 = 1;
			}
		}
	
		


	// Load the vertex array with data.

	//vertices[0].position = XMFLOAT3(-3.0f, 0.0f, 0.0f);  // Bottom left 1.
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = XMFLOAT3(-3.0f, 3.0f, 0.0f);  // Top left 1.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = XMFLOAT3(0.0f, 3.0f, 0.0f);  // Top right 1.
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[3].position = XMFLOAT3(0.0f, 0.0f, 0.0f);  // Bottom right 1.
	//vertices[3].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);


	// Load the index array with data.

	/*indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;*/


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}