#pragma once
#ifndef __EMSCRIPTEN__
#include <irrKlang.h>
#endif
#include <Camera/Camera.h>

namespace Fed
{
	#define Audio AudioEngine::Instance()
	class AudioEngine
	{
	public:
		static AudioEngine& Instance();
		AudioEngine();
		~AudioEngine();
		void SetCameraReference(Camera* camera) { m_Camera = camera; }

		void PlaySound(std::string file);
		void PlaySound3D(std::string file, Vector3 position);
		void PlayMusic(std::string file);

		void SetAudioEnabled(bool enabled);
		bool GetAudioEnabled() const;
	private:
		const std::string m_Directory = "../Assets/Sounds/";
#ifndef __EMSCRIPTEN__
		irrklang::ISoundEngine* m_Engine;
		irrklang::ISound* m_Music;
#endif
		Camera* m_Camera;
		bool m_Enabled;
	};
}
