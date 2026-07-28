#ifndef CELL_GAMEOBJECT_H
#define CELL_GAMEOBJECT_H

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CellGameObject {
private:
	const char* WITId = "";
protected:
	ID3D11Buffer* constantBuffer;
public:
	virtual CellTransformation* GetVramData()
	{
		return vramData[WITId];
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
		ConstantBuffer cbuffer = {};

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
		ConstantBuffer cbuffer = {};
		return cbuffer;
	}
	virtual void Update()
	{

	}
};

class SimpleRenderCellGameObject:public CellGameObject {
private:
	const char* WISId = "Torus.001";
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
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraTarget, upDirection);
		float fov = 45.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		glm::mat4 projMatrix = glm::perspective(
			glm::radians(fov),
			aspectRatio,
			nearPlane,
			farPlane
		);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		ConstantBuffer cbuffer = {
			glm::transpose(viewMatrix),
			glm::transpose(projMatrix),
			glm::transpose(model)
		};
		return cbuffer;
	}
};


class HaveParentAndChildObject :public SimpleRenderCellGameObject {
private:
	const char* WISId = "Cone";
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
		auto cbuffer = SimpleRenderCellGameObject::GetData();
		return cbuffer;
	}
};

class ExampleCellGameObject :public HaveParentAndChildObject {
private:
	const char* WISId = "Body_2";
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
		auto cbuffer = SimpleRenderCellGameObject::GetData();
		return cbuffer;
	}
};
std::unordered_map<std::string, CellGameObject*> gameObjects;

#endif