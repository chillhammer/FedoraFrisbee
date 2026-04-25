#include <FedPCH.h>
#include <EventSystem/Events/ApplicationEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/EventType.h>
#include <Input/InputManager.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Game/GameManager.h>
#include <stb_image/stb_image.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#ifdef _WIN32
#include <Windows.h>
#endif

#include "Window.h"

static bool s_GLFWInitialized = false;
static void GLFWErrorCallback(int error, const char* description) {
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

Fed::Window::Window(const WindowProps & props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	LOG("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

	// Removing Console
	#ifdef FED_DIST
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	FreeConsole();
	#endif

	if (!s_GLFWInitialized) {
		// TODO: glfwTerminate on system shutdown
		int success = glfwInit();
		ASSERT(success, "Could not initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		s_GLFWInitialized = true;
#ifdef __EMSCRIPTEN__
		// Emscripten's GLFW emulation backs the canvas with WebGL2 directly when
		// USE_WEBGL2=1 is set at link time. The version/profile hints are ignored;
		// Keep MSAA off on web; it is a noticeable cost on integrated GPUs.
		glfwWindowHint(GLFW_SAMPLES, 0);
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
#endif
	}
	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glewInit();
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);

	// Icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("../Assets/Textures/Icon.png", &images[0].width, &images[0].height, 0, 4);
	//rgba channels 
	glfwSetWindowIcon(m_Window, 1, images);
	stbi_image_free(images[0].pixels);

	//GLCall(glEnable(GL_BLEND));
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glViewport(0, 0, props.Width, props.Height);

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDepthFunc(GL_LESS));
#ifdef __EMSCRIPTEN__
	// glDepthRange takes floats in GLES3; glDepthRangef is the spec-correct call.
	GLCall(glDepthRangef(0.f, 1.f));
	// GL_MULTISAMPLE has no equivalent in GLES3 — antialiasing is a context attribute.
#else
	GLCall(glDepthRange(0.f, 1.f));
	GLCall(glEnable(GL_MULTISAMPLE));
#endif

	m_Data.EventCallback = EVENT_BIND_FN(Window, OnEvent);

	// Set GLFW callbacks, Event System
	#pragma region "GLFW Callbacks"
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			data.EventCallback(event);
			break;
		}

		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
		}
	});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		KeyTypedEvent event(keycode);
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.EventCallback(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float)xPos, (float)yPos);
		data.EventCallback(event);
	});
	#pragma endregion
}

void Fed::Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Fed::Window::OnEvent(Event & e)
{
	// Resizing should be handled locally
	Evnt::Dispatch<WindowResizeEvent>(e, EVENT_BIND_FN(Window, OnWindowResized));
	// Event Handling, Call Appropriate Subject
	switch (e.GetEventType())
	{
	case EventType::WindowClose:
		Game.Shutdown();
		break;
	case EventType::MouseMoved:
		Input.MouseMoved.Notify(e);
		break;
	case EventType::MouseButtonPressed:
		Input.MouseClicked.Notify(e); 
		break;
	case EventType::MouseButtonReleased:
		Input.MouseReleased.Notify(e);
		break;
	case EventType::MouseScrolled:
		Input.MouseScrolled.Notify(e);
		break;
	case EventType::KeyPressed:
		Input.KeyPressed.Notify(e);
		break;
	case EventType::KeyReleased:
		Input.KeyReleased.Notify(e);
		break;
	}
}

void Fed::Window::SetCursorEnabled(bool enabled) const
{
	if (!enabled)
	{
#ifdef __EMSCRIPTEN__
		emscripten_run_script("if (window.FedoraFrisbeeSetCursorVisible) window.FedoraFrisbeeSetCursorVisible(false);");
#endif
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
#ifdef __EMSCRIPTEN__
		emscripten_run_script("if (window.FedoraFrisbeeSetCursorVisible) window.FedoraFrisbeeSetCursorVisible(true);");
#endif
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Fed::Window::SetCursorPosition(float x, float y) const
{
	glfwSetCursorPos(m_Window, x, y);
	Input.SetMousePosition(Vector2(x, y));
}

bool Fed::Window::OnWindowResized(WindowResizeEvent & e)
{
	LOG("Window Resized: ({0}, {1})", e.GetWidth(), e.GetHeight());
	//m_Data.Width = e.GetWidth();
	//m_Data.Height = e.GetHeight();
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	WindowResized.Notify(e);
	return true;
}

Fed::Window::~Window()
{
	glfwDestroyWindow(m_Window);
}
