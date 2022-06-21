#include "Audio/AudioManager.h"
namespace GameEngine {

    AudioManager* AudioManager::instance = nullptr;

    AudioManager::AudioManager()
    {
        // find the default audio device
        const ALCchar* defaultDeviceString = alcGetString(/*device*/nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        device = alcOpenDevice(defaultDeviceString);
        if (!device)
        {
            std::cerr << "failed to get the default device for OpenAL" << std::endl;
            success = false;
        }
        else
        {
            std::cout << "OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;

            // Create an OpenAL audio context from the device
            context = alcCreateContext(device, /*attrlist*/ nullptr);

            // Activate this context so that OpenAL state modifications are applied to the context
            success = alcMakeContextCurrent(context);

            if (!success)
            {
                std::cerr << "failed to make the OpenAL context the current context" << std::endl;
            }
            else
            {
                // Create a listener in 3d space (ie the player); (there always exists as listener, you just configure data on it)
                alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
                alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
                ALfloat forwardAndUpVectors[] = {
                    /*forward = */ 1.f, 0.f, 0.f,
                    /* up = */ 0.f, 1.f, 0.f
                };
                alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
            }
        }
    }

    AudioManager::~AudioManager()
    {
    }

    // clean up our resources!
    void AudioManager::clean()
    {
        for (auto x : m_monoAudios)
        {
            ALuint monoSource = x.second.second;
            ALuint monoSoundBuffer = x.second.first;
            alec(alDeleteSources(1, &monoSource));
            alec(alDeleteBuffers(1, &monoSoundBuffer));
        }

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);

        delete instance;
    }

    AudioManager* AudioManager::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new AudioManager();
        }

        return instance;
    }

    int AudioManager::readMonoData(std::string filename)
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

        // create a sound source that plays our mono sound (from the sound buffer)
        ALuint monoSource;
        alec(alGenSources(1, &monoSource));
        alec(alSource3f(monoSource, AL_POSITION, 1.f, 0.f, 0.f));
        alec(alSource3f(monoSource, AL_VELOCITY, 0.f, 0.f, 0.f));
        alec(alSourcef(monoSource, AL_PITCH, 1.f));
        alec(alSourcef(monoSource, AL_GAIN, 1.f));
        alec(alSourcei(monoSource, AL_LOOPING, AL_FALSE));
        alec(alSourcei(monoSource, AL_BUFFER, monoSoundBuffer));

        m_monoAudios.insert({ filename, {monoSoundBuffer, monoSource} });

        return 0;
    }

    // play the mono sound source
    void AudioManager::playMonoSound(std::string filename)
    {
        ALuint monoSource = m_monoAudios[filename].second;

        alec(alSourcePlay(monoSource));
        ALint sourceState;
        alec(alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState));
    }

    void AudioManager::loopMonoSound(std::string filename)
    {
        ALuint monoSource = m_monoAudios[filename].second;
        alSourcei(monoSource, AL_LOOPING, 1);
        alec(alSourcePlay(monoSource));
        ALint sourceState;
        alec(alGetSourcei(monoSource, AL_SOURCE_STATE, &sourceState));
    }

    void AudioManager::stopLoopMonoSound(std::string filename)
    {
        ALuint monoSource = m_monoAudios[filename].second;
        alSourcei(monoSource, AL_LOOPING, 0);
    }

    void AudioManager::stopMonoSound(std::string filename)
    {
        ALuint monoSource = m_monoAudios[filename].second;
        alSourceStop(monoSource);
    }
}