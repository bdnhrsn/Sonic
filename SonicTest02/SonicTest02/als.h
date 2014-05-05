#ifndef AL_S_H
#define AL_S_H

#include <string>
#include "al.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


	namespace utility {

		// Structs for components of a WAVE file
		typedef struct RIFF_Header;
		typedef struct WAVE_Format;
		typedef struct WAVE_Data;

		// Load WAVE file function
		bool loadWAVFile(const std::string fname, ALuint* buffer,
			ALsizei* size, ALsizei* frequency, ALenum* format);



	}
#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* AL_S_H */