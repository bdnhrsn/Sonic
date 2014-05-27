#ifndef WAV_H
#define WAV_H

#include <string>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


	namespace utility {

		// Structs for components of a WAVE file
		typedef struct RIFF_Header;
		typedef struct WAVE_Format;
		typedef struct WAVE_Data;

		// Load WAVE file function
		bool loadWAVFile(const std::string fname, void* buffer, int* size, int* frequency, short bitd, short channels);
	}
#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* AL_S_H */