#pragma once
#include "GLFW/glfw3.h"
#include <string>

struct WindowProp
{
	int width;
	int height;
	const char* name;
};

class Window
{
public:
	Window(WindowProp props);
	~Window();

	static Window* Create(WindowProp);
	void Update();

private:
	bool Init() const;
	void Shutdown() const;
	bool CreateWindow();
	void OnUpdate();
	void UseVSync(bool value);

	GLFWwindow* m_Window;
	WindowProp m_Props;
};

