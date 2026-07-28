#ifndef CELL_GFX_H
#define CELL_GFX_H
//グラフィックのヘッダーが読み込まれでいない場合

#ifndef GFX_LOAD_MISSING
#error The graphic header has not been loaded.
#endif

#include <random>
#include <loader/tiny_gltf.h>
#include <sstream>

//グラフィックのヘッダーが読み込まれている場合


class CellGFX:Trampoline {
public:
	void Run()
	{
		Initialize();
		WriteVertexMemory("G://CellGPU//CMakeProject4//ExampleData//SomeBox.glb","SomeBox");
		//riteVertexMemory("G://CellGPU//CMakeProject4//ExampleData//Kuromoji_v12.0_Basic.glb","Kuromoji");
		CreateBuffer();
		BindData();
		Render();
	}
	void WriteVertexMemory(std::string path,std::string uniqueID)
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string err;
		std::string warn;

		bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
		if (!ret)
		{
			std::cout << err << std::endl;
			std::cout << warn << std::endl;
		}
		
		struct VramData {
			std::string id;
			std::vector<glm::vec3> vercites;
			std::vector<unsigned char> indices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> uv;
			std::vector<std::string> children;
		};

		std::vector<VramData> vramDatas(0);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distrib(1, 10000);
		int index = 0;
		for (auto node : model.nodes)
		{
			std::stringstream id;
			id << node.name;

			VramData pushData;
			pushData.id = id.str();
			if (node.mesh == -1)
			{
				continue;
			}
			for (auto primitive : model.meshes[node.mesh].primitives)
			{
				for (auto attribute : primitive.attributes) {
					if (attribute.first == "POSITION")
					{
						tinygltf::BufferView view = model.bufferViews[model.accessors[attribute.second].bufferView];
						std::vector<unsigned char> data(model.buffers[view.buffer].data.begin() + view.byteOffset, model.buffers[view.buffer].data.begin() + view.byteOffset + view.byteLength);
						size_t numVecs = data.size() / sizeof(glm::vec3);
						pushData.vercites.resize(numVecs);
						std::memcpy(pushData.vercites.data(), data.data(), numVecs * sizeof(glm::vec3));
					}
					if (attribute.first == "NORMAL")
					{
						tinygltf::BufferView view = model.bufferViews[model.accessors[attribute.second].bufferView];
						std::vector<unsigned char> data(model.buffers[view.buffer].data.begin() + view.byteOffset, model.buffers[view.buffer].data.begin() + view.byteOffset + view.byteLength);
						size_t numVecs = data.size() / sizeof(glm::vec3);
						pushData.normals.resize(numVecs);
						std::memcpy(pushData.normals.data(), data.data(), numVecs * sizeof(glm::vec3));
					}
					if (attribute.first == "TEXCOORD_0")
					{
						tinygltf::BufferView view = model.bufferViews[model.accessors[attribute.second].bufferView];
						std::vector<unsigned char> data(model.buffers[view.buffer].data.begin() + view.byteOffset, model.buffers[view.buffer].data.begin() + view.byteOffset + view.byteLength);
						size_t numVecs = data.size() / sizeof(glm::vec2);
						pushData.uv.resize(numVecs);
						std::memcpy(pushData.uv.data(), data.data(), numVecs * sizeof(glm::vec2));
					}
				}
			}
			for (auto child : node.children)
			{
				pushData.children.push_back(model.nodes[child].name);
			}

			int type = model.accessors[model.meshes[node.mesh].primitives[0].indices].componentType;

			tinygltf::BufferView indexView = model.bufferViews[model.accessors[model.meshes[node.mesh].primitives[0].indices].bufferView];
			std::vector<unsigned char> indicesData(model.buffers[indexView.buffer].data.begin() + indexView.byteOffset, model.buffers[indexView.buffer].data.begin() + indexView.byteOffset + indexView.byteLength);
			pushData.indices = indicesData;
			vramDatas.push_back(pushData);
		}
		for (auto data : vramDatas) {
			std::cout << data.id << std::endl;
			vramData[data.id] = new CellTransformation(uniqueID, data.vercites,data.indices,data.children);
		}
	}
};

#endif