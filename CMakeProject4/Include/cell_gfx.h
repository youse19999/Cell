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

template <typename T>
void readAttributeValue(const uint8_t* src, int componentType, T* dest, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		switch (componentType) {
		case TINYGLTF_COMPONENT_TYPE_FLOAT:
			dest[i] = static_cast<T>(reinterpret_cast<const float*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_DOUBLE:
			dest[i] = static_cast<T>(reinterpret_cast<const double*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			dest[i] = static_cast<T>(reinterpret_cast<const uint8_t*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_BYTE:
			dest[i] = static_cast<T>(reinterpret_cast<const int8_t*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			dest[i] = static_cast<T>(reinterpret_cast<const uint16_t*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_SHORT:
			dest[i] = static_cast<T>(reinterpret_cast<const int16_t*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			dest[i] = static_cast<T>(reinterpret_cast<const uint32_t*>(src)[i]);
			break;
		case TINYGLTF_COMPONENT_TYPE_INT:
			dest[i] = static_cast<T>(reinterpret_cast<const int32_t*>(src)[i]);
			break;
		default:
			dest[i] = static_cast<T>(0);
			break;
		}
	}
}

class CellGFX :Trampoline {
public:
	void Run()
	{
		Initialize();
		WriteVertexMemory("D://25ci0513//Shiromoji_v12.1_Blender.glb", "Shiromoji");
		CreateBuffer();
		BindData();
		Render();
	}
	struct AttributeAccessor {

		const uint8_t* ptr = nullptr;
		size_t stride = 0;
		int componentType = 0;
		int numComponents = 0;
		bool isVaild = false;

		template <typename T>
		void read(size_t index, T* dest, size_t targetComponentCount) const {
			//ターゲットのポインタが存在しない、または有効でない場合は、ターゲットに0を書き込み
			if (!isVaild || !ptr)
			{
				std::memset(dest, 0, sizeof(T) * targetComponentCount);
				return;
			}
			const uint8_t* elementPtr = ptr + (index * (targetComponentCount)*sizeof(T));
			size_t countToRead = std::min(static_cast<size_t>(numComponents), targetComponentCount);

			readAttributeValue(elementPtr, componentType, dest, countToRead);
		}
	};
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
				std::cout << index << std::endl;
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
					auto getAttributeAccessor = [&](const std::string& attrName) -> AttributeAccessor {
						auto it = primitive.attributes.find(attrName);
						//attributesが終了、つまり何もない場合
						if (it == primitive.attributes.end())
						{
							return AttributeAccessor{};
						}

						int accIdx = it->second;
						//アクセスのためのIDが0を下回りで、アクセスidがアクセサーの全サイズを超えている場合。
						if (accIdx < 0 || accIdx >= static_cast<int>(model.accessors.size()))
						{
							std::cout << "The Attribute accessor was invalid." << std::endl;
							return AttributeAccessor{};
						}
						const auto& acc = model.accessors[accIdx];
						//アクセスも同様に不正な値なら返す。
						if (acc.bufferView < 0 || acc.bufferView >= static_cast<int>(model.bufferViews.size())) {
							std::cout << "The buffer view was invalid." << std::endl;
							return AttributeAccessor{};
						}
						const auto& view = model.bufferViews[acc.bufferView];
						if (view.buffer < 0 || view.buffer >= static_cast<int>(model.buffers.size())) {
							std::cout << "The buffer was invalid.." << std::endl;
							return AttributeAccessor{};
						}

						const auto& buf = model.buffers[view.buffer];

						AttributeAccessor attr;
						//アクセスするポインタの先（データの位置＋ビューが指定するオフセット（つまり、そのバッファの位置）＋アクセサにおける属性の始まりのオフセット
						attr.ptr = buf.data.data() + view.byteOffset + acc.byteOffset;
						attr.stride = 1;
						attr.componentType = acc.componentType;
						//アクセサのidからコンポーネントの数を教えてくれる。
						attr.numComponents = tinygltf::GetNumComponentsInType(acc.type);
						attr.isVaild = true;
						return attr;
					};
					bool useAnimMesh = true;
					//テックスコードで終了するなら
					auto it = primitive.attributes.find("JOINTS_0");
					if (it == primitive.attributes.end()) {
						useAnimMesh = false;
					}
					if (useAnimMesh) {
						tdata->hasSkin = true;
					}
					else{
						tdata->hasSkin = false;
					}

					AttributeAccessor posAttr = getAttributeAccessor("POSITION");
					AttributeAccessor normAttr = getAttributeAccessor("NORMAL");
					AttributeAccessor uvAttr = getAttributeAccessor("TEXCOORD_0");
					AttributeAccessor jointAttr = getAttributeAccessor("JOINTS_0");
					AttributeAccessor weightAttr = getAttributeAccessor("WEIGHTS_0");

					auto posIt = primitive.attributes.find("POSITION");
					if (posIt == primitive.attributes.end()) {
						return;
					}

					size_t vertexCount = model.accessors[posIt->second].count;
					tdata->vertex.resize((tdata->vertex.size()) + vertexCount);
					for (size_t i = (tdata->vertex.size()- vertexCount); i < (tdata->vertex.size()- vertexCount)+vertexCount; ++i) {
						//カウンタ・verticesの位置、サイズでそのまま読み取られるため、横に並ぶ。
						posAttr.read(i, tdata->vertex[i].position, 3);
						normAttr.read(i, tdata->vertex[i].normal, 3);
						uvAttr.read(i, tdata->vertex[i].uv, 2);
						jointAttr.read(i, tdata->vertex[i].joint, 4);
						weightAttr.read(i, tdata->vertex[i].weight, 4);
					}
				}

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
			index++;

		}
	}
};

#endif
