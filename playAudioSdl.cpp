#include <cstring>
#include <cmath>
#include <string>

#include <SDL.h>
#include "proAudioSdl.h"

using namespace std;

//--- main ---------------------------------------------------------

int main(int argc, char **argv) {
	if(argc<2) {
		fprintf(stderr, "usage: %s audiofile [volumeFactor] [pitchFactor]\n", argv[0]);
		return 1;
	}
    DeviceAudio* audio=DeviceAudioSdl::create();
	if(!audio) {
		fprintf(stderr, "ERROR opening audio device. Abort.\n");
		return 1;
	}
    
	float volume = (argc>2) ? atof(argv[2]) : 1.0f;
	float pitch = (argc>3) ? atof(argv[3]) : 1.0f;
	unsigned int sample1=audio->sampleFromFile(argv[1], volume);
	if(sample1) audio->soundPlay(sample1, 1.0f,1.0f,0.0f, pitch);

    // main loop:
    while(audio->soundActive())
        SDL_Delay(10);
    return 0;
}
