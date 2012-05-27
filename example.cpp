#include "proAudioRt.h"

int main( int argc, char **argv ) {
	// create an audio device using the RtAudio backend and default parameters:
    DeviceAudio* audio=DeviceAudioRt::create();
	if(!audio) return 1; // exit in case of errors

	// load and play a sample:
	unsigned int sample = audio->sampleFromFile("sample.ogg");
	if(sample) audio->soundPlay(sample);

    // wait until the sound has finished:
    while(audio->soundActive()); // for the sake of simplicity busy waiting instead of a preferable sleep() call

	// cleanup and exit:
	audio->destroy();
	return 0;
}
