#pragma once
#include "GLFW/glfw3.h"
#include "GameEngineOne/Events/Event.h"

namespace Fed
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string&title = "Fedora Frisbee",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	/**
		Window class is a wrapper for GLFW's window, using m_Data to transfer
		data between callback function and main class properties
	**/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		~Window() {}

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; };
		unsigned int GetHeight() const { return m_Data.Height; }

		// Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) {
			m_Data.EventCallback = callback;
		};
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const { return m_Data.VSync; }
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};
}