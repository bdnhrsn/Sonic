#include "World.h"
#include <stdlib.h>

World::World() {

}

World::World(int width, int height) {
	setGridDimension(width, height);
}

World::~World(){
	delete[] grid;
}

void World::setGridDimension(int width, int height) {
	
	for (int i = 0; i < width; i++) {
		grid[i] = (int *)malloc(height);
	}
}