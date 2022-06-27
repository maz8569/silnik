#include "Audio/SoundBuffer.h"
#include <inttypes.h>
#include <AL\alext.h>

namespace GameEngine {

	SoundBuffer* SoundBuffer::get()
	{
		static SoundBuffer* sndbuf = new SoundBuffer();
		return sndbuf;
	}

	ALuint SoundBuffer::addSoundEffect(std::string filename)
	{

		ReadWavData monoData;
		{
			std::string filepath = "res/sounds/" + filename + ".wav";
			drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(filepath.c_str(), &monoData.channels, &monoData.sampleRate, &monoData.totalPCMFrameCount, nullptr);
			if (pSampleData == NULL) {
				std::cerr << "failed to load audio file" << std::endl;
				drwav_free(pSampleData, nullptr); //todo use raii to clean this up
				return -1;
			}
			if (monoData.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
			{
				std::cerr << "too much data in file for 32bit addressed vector" << std::endl;
				drwav_free(pSampleData, nullptr);
				return -1;
			}
			monoData.pcmData.resize(size_t(monoData.getTotalSamples()));
			std::memcpy(monoData.pcmData.data(), pSampleData, monoData.pcmData.size() * /*twobytes_in_s16*/2);
			drwav_free(pSampleData, nullptr);
		}

		ALuint monoSoundBuffer;
		alec(alGenBuffers(1, &monoSoundBuffer));
		alec(alBufferData(monoSoundBuffer, monoData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, monoData.pcmData.data(), monoData.pcmData.size() * 2 /*two bytes per sample*/, monoData.sampleRate));

		return monoSoundBuffer;
	}

	bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
	{
		auto it = p_SoundEffectBuffers.begin();
		while (it != p_SoundEffectBuffers.end())
		{
			if (*it == buffer)
			{
				alDeleteBuffers(1, &*it);

				it = p_SoundEffectBuffers.erase(it);

				return true;
			}
			else {
				++it;
			}
		}
		return false;  // couldn't find to remove
	}


	SoundBuffer::SoundBuffer()
	{
		p_SoundEffectBuffers.clear();

	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());

		p_SoundEffectBuffers.clear();
	}

}