#pragma once

#include <Judas-Engine/Layers/Layer.h>
#include <Judas-Engine/Renderer/PerspectiveCameraController.h>

// TEMP
#include <Judas-Engine/Logging/Log.h>

class TestManagerLayer : public Judas_Engine::Layer
{
public:
	TestManagerLayer() : Layer("MyLayer"), m_CameraController(1.0f, 1280.0f / 720.0f, 0.1f, 100.0f) {}
	~TestManagerLayer() {}

	virtual void OnImGuiRender() override;
	virtual void OnEvent(Judas_Engine::Event& e) override;
	virtual void OnUpdate(Judas_Engine::Timestep ts) override;

	void AddTestLayer(const std::string& name, const Judas_Engine::Ref<Judas_Engine::Layer>& testLayer) { m_TestMap[name] = testLayer; JE_INFO("{0}", m_TestMap[name]->GetName()); }
private:
	std::unordered_map<std::string, Judas_Engine::Ref<Judas_Engine::Layer>> m_TestMap;
	Judas_Engine::Ref<Judas_Engine::Layer> m_SelectedTest;

	Judas_Engine::PerspectiveCameraController m_CameraController;
};
