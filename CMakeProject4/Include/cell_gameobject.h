#ifndef CELL_GAMEOBJECT_H
#define CELL_GAMEOBJECT_H

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CellGameObject {
private:
	const char* WISId = "triangle";
protected:
	ID3D11Buffer* constantBuffer;
public:
	virtual CellTransformation* GetVramData()
	{
		return vramData[WISId];
	}
	virtual ID3D11Buffer* GetConstantBuffer()
	{
		return constantBuffer;
	}
	virtual void UpdateConstantBuffer(ID3D11DeviceContext* context)
	{
		ConstantBuffer cbuffer = GetData();
		context->UpdateSubresource(
			constantBuffer,
			0,
			NULL,
			&cbuffer,
			0,
			0
		);
	}
	void CreateConstantBuffer(ID3D11Device* device)
	{
		ConstantBuffer cbuffer = { glm::vec4(1.0f,0.0f,0.0f,1.0f) };

		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(ConstantBuffer);
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = &cbuffer;

		HRESULT result = device->CreateBuffer(&buffer_desc, &subresource_data, &constantBuffer);
		if (FAILED(result))
		{
			return;
		}
	}
	virtual ConstantBuffer GetData()
	{
		glm::mat4 model = glm::mat4(1.0f);
		
		ConstantBuffer cbuffer = {
			{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
			glm::transpose(model)
		};
		return cbuffer;
	}
	virtual void Update()
	{

	}
};

class ExampleCellGameObject:public CellGameObject {
private:
	const char* WISId = "triangle";
public:
	virtual CellTransformation* GetVramData()
	{
		return vramData[WISId];
	}
	void Update() override
	{
		
	}
	ConstantBuffer GetData() override
	{
		glm::mat4 model = glm::mat4(1.0f);
		ConstantBuffer cbuffer = { 
			glm::vec4(1.0f,0.0f,1.0f,1.0f),
			glm::transpose(model)
		};
		return cbuffer;
	}
};

std::unordered_map<const char*, CellGameObject*> gameObjects;

#endif