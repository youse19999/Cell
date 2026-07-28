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
		WriteVertexMemory("G://CellGPU//CMakeProject4//ExampleData//Kuromoji_v12.0_Basic.glb","Kuromoji");
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

		int index = 0;
		for (auto node : model.nodes)
		{
			std::stringstream id;
			id << node.name;

			TransformationData* tdata = new TransformationData();
			tdata->id = uniqueID.c_str();
			if (node.mesh == -1)
			{
				continue;
			}
			std::vector<float> mergedPositions;
			for (auto primitive : model.meshes[node.mesh].primitives)
			{
				for (auto attribute : primitive.attributes) {
					tinygltf::BufferView view = model.bufferViews[model.accessors[attribute.second].bufferView];
					tinygltf::Buffer& buffer = model.buffers[view.buffer];
					if (attribute.first == "POSITION")
					{
						const unsigned char* src = buffer.data.data() + view.byteOffset;
						size_t size = view.byteLength;
						std::vector<float> g;
						g.resize(size);
						std::memcpy(g.data(), src, size);
						tdata->vertex.assign(tdata->vertex.begin(), tdata->vertex.end());
						mergedPositions.insert(mergedPositions.end(), g.begin(), g.end());
						std::cout << mergedPositions.size() << std::endl;
					}
					if (attribute.first == "NORMAL")
					{
						//std::memcpy(tdata.vertex.data(), data.data(), data.size());
					}
					if (attribute.first == "TEXCOORD_0")
					{
						//std::memcpy(tdata.vertex.data(), data.data(), data.size());
					}
				}
			}
			tdata->vertex.resize(mergedPositions.size());
			std::memcpy(tdata->vertex.data(), mergedPositions.data(), mergedPositions.size());

			int type = model.accessors[model.meshes[node.mesh].primitives[0].indices].componentType;

			tinygltf::Accessor& accessor = model.accessors[model.meshes[node.mesh].primitives[0].indices];
			tinygltf::BufferView& indexView = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer& buffer = model.buffers[indexView.buffer];
			size_t componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType);
			size_t numComponents = tinygltf::GetNumComponentsInType(accessor.type);
			size_t dataSize = accessor.count * numComponents * componentSize;

			const unsigned char* src = buffer.data.data() + indexView.byteOffset + accessor.byteOffset;

			tdata->index.resize(dataSize);
			std::memcpy(tdata->index.data(), src, dataSize);

			tdata->indexType = accessor.componentType;
			vramData[node.name] = new CellTransformation(tdata);


		}
	}
};

#endif
