#pragma once

#include "Application.h"

int main(int argc, char** argv)
{
    Judas_Engine::Application* app = new Judas_Engine::Application();
    app->Run();
    delete app;
}