#ifndef PLAYER_H
#define PLAYER_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

	class Player {
		public:
			Player();
			Player(int pos[3]);
			~Player();
			int* getCoord();
			void setCoord(int pos[3]);
		protected:
			int coord[3];
	};

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* PLAYER_H */