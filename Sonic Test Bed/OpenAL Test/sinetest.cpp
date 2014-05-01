#include "AL\al.h"
#include "AL\alc.h"
#include <stdio.h>
#include <string>

using namespace std;

static void list_audio_devices(const ALCchar *devices)
{
        const ALCchar *device = devices, *next = devices + 1;
        size_t len = 0;

        fprintf(stdout, "Devices list:\n");
        fprintf(stdout, "----------\n");
        while (device && *device != '\0' && next && *next != '\0') {
                fprintf(stdout, "%s\n", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
        }
        fprintf(stdout, "----------\n");
}

int main()
{
	ALCdevice *device;
	device = alcOpenDevice(NULL);
	if(!device);

	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE);

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	alcCloseDevice(device);
	return 0;
}
