#ifndef CELL_WIT_DATA_H
#define CELL_WIT_DATA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class WITData {
public:
	virtual std::vector<glm::vec3> GetVertexBuffer()
	{
		std::vector<glm::vec3> vertices = {
				 glm::vec3(0,0.5f,0),
				 glm::vec3(0.5f,-0.5f,0),
				 glm::vec3(-0.5f,-0.5f,0),
		};
		return vertices;
	}
	virtual glm::vec4 GetColor()
	{
		glm::vec4 color = glm::vec4(1.0f,0.0f,1.0f,1.0);
		return color;
	}
};
class WITData2:public WITData {
public:
	std::vector<glm::vec3> GetVertexBuffer() override
	{
		std::vector<glm::vec3> vertices = {
				 glm::vec3(0,0.25f,0),
				 glm::vec3(1.5f,-0.5f,0),
				 glm::vec3(-0.5f,-0.5f,0),
		};
		return vertices;
	}
};

std::unordered_map<const char*, WITData*> cellDataMap;

#endif