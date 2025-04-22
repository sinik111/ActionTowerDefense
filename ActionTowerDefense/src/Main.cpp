#include "framework.h"

#include "GameApp.h"
#include "Debug.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    Debug::EnableLeakCheck();

    GameApp app;

    app.Initialize();

    app.Run();

    app.Shutdown();
}