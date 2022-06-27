#pragma once

#include <AL/alc.h>

namespace GameEngine {

	class SoundDevice
	{
	public:
		static SoundDevice* get();
	private:
		SoundDevice();
		~SoundDevice();

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;

	};

}