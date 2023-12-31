#include <JudasEngine.h>

#include "Assets/Scripts/TestManager.h"
#include "Assets/Scripts/Tests/PrimitiveTest.h"
#include "Assets/Scripts/Tests/ComputeShaderTest.h"
#include "Assets/Scripts/Tests/TextureTest.h"
#include "Assets/Scripts/Tests/PhillipsTest.h"
#include "Assets/Scripts/Tests/ButterflyTextureTest.h"
#include "Assets/Scripts/Tests/SsboTest.h"
#include "Assets/Scripts/Tests/WaveField.h"
#include "Assets/Scripts/Tests/TextureSwitchTest.h"

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{
		TestManagerLayer* testManagerLayer = new TestManagerLayer();
		testManagerLayer->AddTestLayer("Primitive", std::make_shared<PrimitiveTestLayer>());
		testManagerLayer->AddTestLayer("Texture", std::make_shared<TextureLayer>());
		testManagerLayer->AddTestLayer("Compute Shader", std::make_shared<ComputeShaderLayer>());
		testManagerLayer->AddTestLayer("Phillips Spectrum", std::make_shared<PhillipsLayer>());
		testManagerLayer->AddTestLayer("Butterfly", std::make_shared<ButterflyLayer>());
		testManagerLayer->AddTestLayer("Ssbo", std::make_shared<SsboLayer>());
		testManagerLayer->AddTestLayer("FFT", std::make_shared<FFTLayer>());
		testManagerLayer->AddTestLayer("Texture Switch", std::make_shared<TextureSwitchLayer>());

		PushLayer(testManagerLayer);
	}
	~GameApplication()
	{

	}
};

Judas_Engine::Application* Judas_Engine::CreateApplication()
{
	return new GameApplication();
}