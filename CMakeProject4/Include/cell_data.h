#ifndef CELL_DATA_H
#define CELL_DATA_H

#define VERTEX_MEMORY_SIZE 256;

#include <memory>
#include <unordered_map>


void SET_UP_CELL_DATA() {
	gameObjects["triangle"] = new ExampleCellGameObject();
}
void RELEASE_ALL_CELL_DATA() {
	for (auto cellData : cellDataMap)
	{
		delete cellData.second;
	}
}

#endif