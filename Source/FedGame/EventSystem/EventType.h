#pragma once
#include <FedPCH.h>
#include <Macros.h>

// Centralized approach to keeping track of all events
// Considered using strings over enums, but ultimately decided this game
//	was small enough.
enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

	// Frisbee Field Controller Events
	FrisbeeThrown, FrisbeePickup
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4),
	EventCategoryFrisbeeField = BIT(5)
};