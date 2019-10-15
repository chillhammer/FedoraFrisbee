#include "FedPCH.h"
#include "AudioEngine.h"

namespace Fed {
	AudioEngine& AudioEngine::Instance()
	{
		{
			static AudioEngine instance;
			return instance;
		}
	}
	AudioEngine::AudioEngine() : m_Camera(nullptr), m_Enabled(true)
	{
		m_Engine = irrklang::createIrrKlangDevice();
	}
	AudioEngine::~AudioEngine()
	{
		if (m_Engine)
			m_Engine->drop();
	}
	void AudioEngine::PlaySound(std::string file)
	{
		if (!m_Enabled) return;
		std::string filePath = (m_Directory + file);
		const char* filePathCString = filePath.c_str();
		m_Engine->setSoundVolume(1.0f);
		auto sound = m_Engine->play2D(filePathCString, false, false, true);
		sound->setVolume(0.25f);
	}
	void AudioEngine::PlaySound3D(std::string file, Vector3 position)
	{
		if (!m_Enabled) return;
		ASSERT(m_Camera, "Must have camera to play 3D sound");
		Vector4 pos = Vector4(position, 1.0f);
		Vector3 localPos = glm::inverse(m_Camera->ObjectTransform.GetMatrix())* pos;
		irrklang::vec3df soundPos(localPos.x, localPos.y, localPos.z);

		std::string filePath = (m_Directory + file);
		const char* filePathCString = filePath.c_str();
		m_Engine->setSoundVolume(1.0f);
		m_Engine->play3D(filePathCString, soundPos);
	}
	void AudioEngine::PlayMusic(std::string file)
	{
		if (m_Music) {
			m_Music->stop();
			m_Music->drop();
		}

		std::string filePath = (m_Directory + file);
		const char* filePathCString = filePath.c_str();
		m_Music = m_Engine->play2D(filePathCString, true, !m_Enabled, true);
		if (m_Music)
			m_Music->setVolume(0.05f);
	}
	void AudioEngine::SetAudioEnabled(bool enabled)
	{
		if (!enabled) {
			if (m_Music)
				m_Music->setIsPaused(true);
		}
		else {
			if (m_Music)
				m_Music->setIsPaused(false);
		}
		m_Enabled = enabled;
	}
	bool AudioEngine::GetAudioEnabled() const
	{
		return m_Enabled;
	}
}