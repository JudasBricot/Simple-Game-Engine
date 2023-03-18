#pragma once

#include "Event.h"

#include <sstream>

namespace Judas_Engine
{
	/*
	* ----- Windows Events -----
	*/

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(WindowsClosed)
			EVENT_CLASS_CATEGORY(WindowsEventCategory)
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height) 
			: m_Width(width), m_Height(height) { }

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		EVENT_CLASS_TYPE(WindowsResized)
		EVENT_CLASS_CATEGORY(WindowsEventCategory)

	private:
		int m_Width, m_Height;
	};

	/*
	* ----- Mouse Events -----
	*/

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_X(x), m_Y(y) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: (" << m_X << ',' << m_Y << ")";
			return ss.str();
		}

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		const float m_X, m_Y;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const int offset)
			: m_Offset(offset) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << m_Offset;
			return ss.str();
		}

		float GetOffset() const { return m_Offset; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

	private:
		const float m_Offset;
	};

	class MouseDraggedEvent : public Event
	{
	public:
		MouseDraggedEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseDragged: (" << m_XOffset << ',' << m_YOffset << ")";
			return ss.str();
		}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseDragged)
			EVENT_CLASS_CATEGORY(MouseEventCategory)

	private:
		const float m_XOffset, m_YOffset;
	};

	/*
	* ----- MouseButton Events -----
	*/

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(const int button)
			: m_Button(button) { }

		inline int GetMouseButton() const { return m_Button; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory | MouseButtonEventCategory)

	private:
		const int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << MouseButtonEvent::ToString() << ", MouseButtonPressedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << MouseButtonEvent::ToString() << ", MouseButtonReleasedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	/*
	* ----- Keyboard Events -----
	*/

	class KeyEvent : public Event
	{
	public:
		KeyEvent(const char keyCode)
			: m_KeyCode(keyCode) {}

		inline char GetKeyCode() { return m_KeyCode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(InputEventCategory | KeyboardEventCategory)

	private:
		char m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const char keyCode, const int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << KeyEvent::ToString() << ", KeyPressedEvent, repeat count: " << m_RepeatCount;
			return ss.str();
		}

		inline int GetRepeatCount() { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const char keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << KeyEvent::ToString() << ", KeyReleasedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
