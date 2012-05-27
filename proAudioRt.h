#include "proAudio.h"
#include <RtAudio.h>
#include <map>

/** @file proAudioRt.h
 \brief RtAudio backend of proteaAudio
 \author  Gerald Franz, www.viremo.de
 \version 0.6
*/ 

struct _AudioTrack;

/// an rtAudio based stereo audio mixer/playback device
/** DeviceAudioRt offers some advanced features such as dynamic pitch,
 independent volume control for both channels, and user-defined time shifts between the channels. */
class DeviceAudioRt : public DeviceAudio {
public:
	///creates audio device
    /** Use this method instead of a constructor.
     \param nTracks (optional) the maximum number of sounds that are played parallely. Computation time is linearly correlated to this factor.
     \param frequency (optional) sample frequency of the playback in Hz. 22050 corresponds to FM radio 44100 is CD quality. Computation time is linearly correlated to this factor.
     \param chunkSize (optional) the number of bytes that are sent to the sound card at once. Low numbers lead to smaller latencies but need more computation time (thread switches). If a too small number is chosen, the sounds might not be played continuously. The default value 512 guarantees a good latency below 40 ms at 22050 Hz sample frequency.
     \return a pointer to an audio device object in case of success
	Note that the parameters are only handled when calling for the first time. Afterwards always the same object is returned until an explicit destroy() is called.
     */
    static DeviceAudio* create(unsigned int nTracks=8, unsigned int frequency=22050, unsigned int chunkSize=1024);

    /// converts a sound sample to internal audio format, returns handle
    virtual unsigned int sampleFromMemory(const AudioSample & sample, float volume=1.0f);
	/// deletes a previously created sound sample resource identified by its handle
	virtual bool sampleDestroy(unsigned int sample);
	/// allows read access to a sample identified by its handle
	virtual const AudioSample* sample(unsigned int handle) const;

	/// plays a specified sample once and sets its parameters
    /** \param sample a sample handle returned by a previous load() call
     \param volumeL (optional) left volume
     \param volumeR (optional) right volume
     \param disparity (optional) time difference between left and right channel in seconds. Use negative values to specify a delay for the left channel, positive for the right.
     \param pitch (optional) pitch factor for playback. 0.5 corresponds to one octave below, 2.0 to one above the original sample.
     \return a handle to the currently played sound or 0 in case of error */
    virtual unsigned int soundPlay(unsigned int sample, float volumeL=1.0f, float volumeR=1.0f, float disparity=0.0f, float pitch=1.0f );
    /** plays a specified sample continuously and sets its parameters
     \param sample a sample handle returned by a previous load() call
     \param volumeL (optional) left volume
     \param volumeR (optional) right volume
     \param disparity (optional) time difference between left and right channel in seconds. Use negative values to specify a delay for the left channel, positive for the right.
     \param pitch (optional) pitch factor for playback. 0.5 corresponds to one octave below, 2.0 to one above the original sample.
     \return a handle to the currently played sound or 0 in case of error */
    virtual unsigned int soundLoop(unsigned int sample, float volumeL=1.0f, float volumeR=1.0f, float disparity=0.0f, float pitch=1.0f );
    /// updates parameters of a specified sound
     /** \param sound  handle of a currently active sound
     \param volumeL left volume
     \param volumeR right volume
     \param disparity (optional) time difference between left and right channel in seconds. Use negative values to specify a delay for the left channel, positive for the right.
     \param pitch (optional) pitch factor for playback. 0.5 corresponds to one octave below, 2.0 to one above the original sample.
     \return true in case the parameters have been updated successfully */
    virtual bool soundUpdate(unsigned int sound, float volumeL, float volumeR, float disparity=0.0f, float pitch=1.0f );
    /// stops a specified sound immediately
    virtual bool soundStop(unsigned int sound);
    /// stops all sounds immediately
    virtual void soundStop();
	/// returns number of currently active sounds
	virtual unsigned soundActive() const;
protected:
    /// constructor. Use the create() method instead
    DeviceAudioRt(unsigned int nTracks, unsigned int frequency, unsigned int chunkSize);
    /// destructor. Use the destroy() method instead
    virtual ~DeviceAudioRt();
	/// mixes tracks to a single output stream
	int mixOutputFloat(signed short *outputBuffer, unsigned int nFrames);

	/// stores loaded sound samples
    std::map<unsigned int, AudioSample*> mm_sample;
    /// stores maximum sample id
    unsigned int m_sampleCounter;

    /// stores sounds to be mixed
    _AudioTrack * ma_sound;
    /// stores number of parallel sounds
    unsigned int m_nSound;
	/// audio manager
	RtAudio m_dac;

    /// mixer callback
    static int cbMix(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *data) {
		return static_cast<DeviceAudioRt*>(data)->mixOutputFloat((signed short*)outputBuffer, nFrames); }
};
