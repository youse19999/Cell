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


class CellGFX :Trampoline {
public:
	void Run()
	{
		Initialize();
		WriteVertexMemory("G://CellGPU//CMakeProject4//ExampleData//SomeBox.glb", "SomeBox");
		WriteVertexMemory("G://CellGPU//CMakeProject4//ExampleData//Kuromoji_v12.0_Basic.glb", "Kuromoji");
		CreateBuffer();
		BindData();
		Render();
	}
	void WriteVertexMemory(std::string path, std::string uniqueID)
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
			for (auto i : node.children)
			{
				tdata->children.push_back(model.nodes[i].name);
			}
			if (node.mesh != -1)
			{
				tdata->isMesh = true;
				std::vector<float> mergedPositions;
				std::vector<float> mergedNormals;
				std::vector<float> mergedUv;
				std::vector<float> mergedWeight;
				std::vector<float> mergedJoint;

				bool jointEnabled = false;
				bool weightEnabled = false;

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
						}
						if (attribute.first == "NORMAL")
						{
							const unsigned char* src = buffer.data.data() + view.byteOffset;
							size_t size = view.byteLength;
							std::vector<float> g;
							g.resize(size);
							std::memcpy(g.data(), src, size);
							mergedNormals.insert(mergedNormals.end(), g.begin(), g.end());
						}
						if (attribute.first == "TEXCOORD_0")
						{
							const unsigned char* src = buffer.data.data() + view.byteOffset;
							size_t size = view.byteLength;
							std::vector<float> g;
							g.resize(size);
							std::memcpy(g.data(), src, size);
							mergedUv.insert(mergedUv.end(), g.begin(), g.end());
						}
						if (attribute.first == "TEXCOORD_0")
						{
							const unsigned char* src = buffer.data.data() + view.byteOffset;
							size_t size = view.byteLength;
							std::vector<float> g;
							g.resize(size);
							std::memcpy(g.data(), src, size);
							mergedUv.insert(mergedUv.end(), g.begin(), g.end());
						}
						if (attribute.first == "JOINTS_0")
						{
							jointEnabled = false;
							const unsigned char* src = buffer.data.data() + view.byteOffset;
							size_t size = view.byteLength;
							std::vector<float> g;
							g.resize(size);
							std::memcpy(g.data(), src, size);
							mergedJoint.insert(mergedJoint.end(), g.begin(), g.end());
						}
						if (attribute.first == "WEIGHTS_0")
						{
							weightEnabled = false;
							const unsigned char* src = buffer.data.data() + view.byteOffset;
							size_t size = view.byteLength;
							std::vector<float> g;
							g.resize(size);
							std::memcpy(g.data(), src, size);
							mergedWeight.insert(mergedWeight.end(), g.begin(), g.end());
						}
					}
				}
				std::vector<unsigned char> gpuFloats;

				std::vector<Vertex> vertices(mergedPositions.size());
				for (size_t i = 0; i < mergedPositions.size() / 3; i++) {
					for (int i2 = 0;i2 < 3;i2++)
					{
						float f = mergedPositions[(i * 3) + i2];
						const auto* p = reinterpret_cast<const unsigned char*>(&f);
						gpuFloats.insert(gpuFloats.end(), p, p + sizeof(float));
						//std::cout << *reinterpret_cast<float*>(bytes) << " ";
					}
					for (int i2 = 0;i2 < 3;i2++)
					{
						float f = mergedNormals[(i * 3) + i2];
						const auto* p = reinterpret_cast<const unsigned char*>(&f);
						gpuFloats.insert(gpuFloats.end(), p, p + sizeof(float));
					}
					for (int i2 = 0;i2 < 2;i2++)
					{
						float f = mergedUv[(i * 2) + i2];
						const auto* p = reinterpret_cast<const unsigned char*>(&f);
						gpuFloats.insert(gpuFloats.end(), p, p + sizeof(float));
					}
					for (int i2 = 0;i2 < 4;i2++)
					{
						if (jointEnabled)
						{
							//gpuFloats.push_back(mergedJoint[(i * 4) + i2]);
						}
					}
					for (int i2 = 0;i2 < 4;i2++)
					{
						if (weightEnabled)
						{
							//gpuFloats.push_back(mergedWeight[(i * 4) + i2]);
						}
					}
				}

				tdata->vertex.resize(gpuFloats.size());
				std::memcpy(tdata->vertex.data(), gpuFloats.data(), gpuFloats.size());

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
			}

			tdata->name = node.name;

			vramData[node.name] = new CellTransformation(tdata);


		}
	}
};

#endif
