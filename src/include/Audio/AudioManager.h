#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include "SoundBuffer.h"
#include <iostream>
#include <vector>
#include <utility>
#include <map>

namespace GameEngine {

	class AudioManager {
	private:
		static AudioManager* instance;
		ALCdevice* device;
		std::map<std::string, std::pair<ALuint, ALuint>> m_monoAudios;
		AudioManager();
		~AudioManager();

	public:

		ALCcontext* context;
		bool success;

		void clean();

		static AudioManager* getInstance();

		//void readStereoData(std::string filename);
		int readMonoData(std::string filename);

		void playMonoSound(std::string filename);
		void loopMonoSound(std::string filename);
		void stopLoopMonoSound(std::string filename);
		void stopMonoSound(std::string filename);
	};

}