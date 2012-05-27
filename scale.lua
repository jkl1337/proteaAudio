-- function creating a sine wave sample:
function sampleSine(freq, duration, sampleRate)
	local data = { }
	for i = 1,duration*sampleRate do
		data[i] = math.sin( (i*freq/sampleRate)*math.pi*2)
	end
	return proAudio.sampleFromMemory(data, sampleRate)
end

-- plays a sample shifted by a number of halftones for a definable period of time
function playNote(sample, pitch, duration, volumeL, volumeR, disparity)
	local scale = 2^(pitch/12)
	local sound = proAudio.soundLoop(sample, volumeL, volumeR, disparity, scale)
	proAudio.sleep(duration)
	proAudio.soundStop(sound)
end


-- create an audio device using default parameters and exit in case of errors
require("proAudioRt")
if not proAudio.create() then os.exit(1) end

-- generate a sample:
local sample = sampleSine(440, 0.5, 88200)

-- play scale (a major):
local duration = 0.5
for i,note in ipairs({ 0, 2, 4, 5, 7, 9, 11, 12 }) do
	playNote(sample, note, duration)
end

-- cleanup
proAudio.destroy()
