#include "JudasEngine.h"

namespace Judas_Bricot
{
    int main(void)
    {
        Judas_Engine::Application* app = Judas_Engine::CreateApplication();
        app->Run();
        delete app;

        return 0;
    }
}
