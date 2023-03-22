#include <JudasEngine.h>


class ExampleLayer : public Judas_Engine::Layer
{
public:
	ExampleLayer()
		: Layer("MyLayer")
	{

	}

	void OnUpdate() override
	{
		JE_INFO("Updating layer");
	}

	void OnEvent(Judas_Engine::Event& e)
	{
		JE_TRACE("{0}", e.ToString());
	}
};

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{

	}
	~GameApplication()
	{

	}
};

Judas_Engine::Application* Judas_Engine::CreateApplication()
{
	return new GameApplication();
}