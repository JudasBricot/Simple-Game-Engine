#include <JudasEngine.h>

#include "Assets/Scripts/TestManager.h"
#include "Assets/Scripts/Layers/SpectraDisplay.h"
#include "Assets/Scripts/Layers/ButterflyTexture.h"
#include "Assets/Scripts/Layers/WaveField.h"

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{
		TestManagerLayer* testManagerLayer = new TestManagerLayer();

		testManagerLayer->AddTestLayer("Spectra Display", std::make_shared<SpectraDisplayLayer>());
		testManagerLayer->AddTestLayer("Butterfly Texture", std::make_shared<ButterflyTextureLayer>());
		testManagerLayer->AddTestLayer("Waves", std::make_shared<WavesLayer>());

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