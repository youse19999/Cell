#ifndef CELL_GFX_H
#define CELL_GFX_H
//グラフィックのヘッダーが読み込まれでいない場合

#ifndef GFX_LOAD_MISSING
#error The graphic header has not been loaded.
#endif

//グラフィックのヘッダーが読み込まれている場合


class CellGFX:Trampoline {
public:
	void Run()
	{
		Initialize();
		WriteVertexMemory();
		BindData();
		Render();
	}
	void WriteVertexMemory()
	{
		glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0);
		float scale = 0.15f;
		std::vector<glm::vec3> vertices = {
				 glm::vec3(0,0.5f,1.0f) * glm::vec3(scale),
				 glm::vec3(0.5f,-0.5f,1.0f) * glm::vec3(scale),
				 glm::vec3(-0.5f,-0.5f,1.0f) * glm::vec3(scale),
		};
		vramData["triangle"] = new CellTransformation("triangle", vertices, color);
		float scale2 = 0.5f;
		std::vector<glm::vec3> vertices2 = {
				 glm::vec3(0,0.5f,1.0f) + glm::vec3(scale2),
				 glm::vec3(0.5f,-0.5f,1.0f) + glm::vec3(scale2),
				 glm::vec3(-0.5f,-0.5f,1.0f) + glm::vec3(scale2),
		};
		vramData["triangle2"] = new CellTransformation("triangle2", vertices2, color);
	}
};

#endif