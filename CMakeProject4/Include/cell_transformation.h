#ifndef CELL_TRANSFORMATION_H
#define CELL_TRANSFORMATION_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

//指定のコンスタントバッファー
struct ConstantBuffer
{
	glm::mat4 view;
	glm::mat4 camera;
	glm::mat4 model;
};

//トランスフォーメーションのデータ
struct TransformationData {
	const char* id;
	//頂点メモリのポインタとしてのID3D11Buffer（COMポインタ）
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	//定数メモリのポインタとしてのID3D11Buffer（COMポインタ）
	std::vector<glm::vec3> vertex;
	std::vector<unsigned char> index;
	std::vector<std::string> children;
	//ここからConstantBuffer分の定数のVRAM
	ConstantBuffer cbuffer;
};


class CellTransformation {
private:
	TransformationData* data;
public:
	unsigned int Size()
	{
		return data->index.size();
	}
	ID3D11Buffer* GetVertexBuffer()
	{
		return data->vertexBuffer;
	}
	ID3D11Buffer* GetIndexBuffer()
	{
		return data->indexBuffer;
	}
	std::vector<std::string> GetChildren()
	{
		if (data->children.size() == 0)
		{
			std::string s = "THIS_OBJECT_DONT_HAVE_CHILDREN!";
			return { s };
		}
		return data->children;
	}
	CellTransformation(TransformationData* data)
	{
		this->data = data;
	}
	void SetData(TransformationData* data)
	{
		this->data = data;
	}
	~CellTransformation()
	{
		std::cout << "release" << std::endl;
		data->vertexBuffer->Release();
		data->indexBuffer->Release();
	}
	void Map(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(glm::vec3) * data->vertex.size();
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = vertexStride * sizeof(float);

		subresourceData = {};
		subresourceData.pSysMem = data->vertex.data();

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, &data->vertexBuffer);

		D3D11_BUFFER_DESC bufferDesc2 = {};
		D3D11_SUBRESOURCE_DATA subresourceData2 = {};

		bufferDesc2 = {};
		bufferDesc2.ByteWidth = data->index.size();
		bufferDesc2.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc2.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subresourceData2 = {};
		subresourceData2.pSysMem = data->index.data();

		hr = device->CreateBuffer(&bufferDesc2, &subresourceData2, &data->indexBuffer);
	}
};

std::unordered_map<std::string, CellTransformation*> vramData;

#endif