#ifndef WORLD_H
#define WORLD_H

#include "Player.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

	class World {
		public:
			World();
			World(int width, int height);
			~World();
			void setGridDimension(int width, int height);
		protected:
			int width, height;
			int** grid;
			Player play;
	};

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* WORLD_H */